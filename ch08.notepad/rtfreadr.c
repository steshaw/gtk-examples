#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gtk/gtk.h>
#include "notepad.h"
#include "rtftype.h"
#include "rtfdecl.h"

int cGroup;
bool fSkipDestIfUnk;
long cbBin;
long lParam;

RDS rds;
RIS ris;

CHP chp;
PAP pap;
SEP sep;
DOP dop;

SAVE *psave;
FILE *fpIn;
FILE *outfp;

//
// %%Function: main
//
// Main loop. Initialize and parse RTF.
//
int ConvertRTF (char *szFilename, char *szOutFile)
{
    FILE *fp;
    int ec;

    fp = fpIn = fopen (szFilename, "r");
    if (!fp) {
        ShowMessage ("Error!", "Can't open file!\n");
        return 1;
    }

    outfp = fopen (szOutFile, "w");

    if ((ec = ecRtfParse(fp)) != ecOK)
        printf("error %d parsing rtf\n", ec);
    fclose (fp);
    fclose (outfp);
    return 0;
}

//
// %%Function: ecRtfParse
//
// Step 1:
// Isolate RTF keywords and send them to ecParseRtfKeyword;
// Push and pop state at the start and end of RTF groups;
// Send text to ecParseChar for further processing.
//

int
ecRtfParse(FILE *fp)
{
    int ch;
    int ec;
    int cNibble = 2;
    int b = 0;
    while ((ch = getc(fp)) != EOF)
    {
        if (cGroup < 0)
            return ecStackUnderflow;
        if (ris == risBin)                      // if we're parsing binary data, handle it directly
        {
            if ((ec = ecParseChar(ch)) != ecOK)
                return ec;
        }
        else
        {
            switch (ch)
            {
            case '{':
                if ((ec = ecPushRtfState()) != ecOK)
                    return ec;
                break;
            case '}':
                if ((ec = ecPopRtfState()) != ecOK)
                    return ec;
                break;
            case '\\':
                if ((ec = ecParseRtfKeyword(fp)) != ecOK)
                    return ec;
                break;
            case 0x0d:
            case 0x0a:          // cr and lf are noise characters...
                break;
            default:
                if (ris == risNorm)
                {
                    if ((ec = ecParseChar(ch)) != ecOK)
                        return ec;
                }
                else
                {               // parsing hex data
                    if (ris != risHex)
                        return ecAssertion;
                    b = b << 4;
                    if (isdigit(ch))
                        b += (char) ch - '0';
                    else
                    {
                        if (islower(ch))
                        {
                            if (ch < 'a' || ch > 'f')
                                return ecInvalidHex;
                            b += (char) ch - 'a';
                        }
                        else
                        {
                            if (ch < 'A' || ch > 'F')
                                return ecInvalidHex;
                            b += (char) ch - 'A';
                        }
                    }
                    cNibble--;
                    if (!cNibble)
                    {
                        if ((ec = ecParseChar(b)) != ecOK)
                            return ec;
                        cNibble = 2;
                        b = 0;
ris = risNorm;
                    }
                }                   // end else (ris != risNorm)
                break;
            }       // switch
        }           // else (ris != risBin)
    }               // while
    if (cGroup < 0)
        return ecStackUnderflow;
    if (cGroup > 0)
        return ecUnmatchedBrace;
    return ecOK;
}

//
// %%Function: ecPushRtfState
//
// Save relevant info on a linked list of SAVE structures.
//

int
ecPushRtfState(void)
{
    SAVE *psaveNew = malloc(sizeof(SAVE));
    if (!psaveNew)
        return ecStackOverflow;

    psaveNew -> pNext = psave;
    psaveNew -> chp = chp;
    psaveNew -> pap = pap;
    psaveNew -> sep = sep;
    psaveNew -> dop = dop;
    psaveNew -> rds = rds;
    psaveNew -> ris = ris;
    ris = risNorm;
    psave = psaveNew;
    cGroup++;
    return ecOK;
}

//
// %%Function: ecPopRtfState
//
// If we're ending a destination (that is, the destination is changing),
// call ecEndGroupAction.
// Always restore relevant info from the top of the SAVE list.
//

int
ecPopRtfState(void)
{
    SAVE *psaveOld;
    int ec;

    if (!psave)
        return ecStackUnderflow;

    if (rds != psave->rds)
    {
        if ((ec = ecEndGroupAction(rds)) != ecOK)
            return ec;
    }
    chp = psave->chp;
    pap = psave->pap;
    sep = psave->sep;
    dop = psave->dop;
    rds = psave->rds;
    ris = psave->ris;

    psaveOld = psave;
    psave = psave->pNext;
    cGroup--;
    free(psaveOld);
    return ecOK;
}

//
// %%Function: ecParseRtfKeyword
//
// Step 2:
// get a control word (and its associated value) and
// call ecTranslateKeyword to dispatch the control.
//

int
ecParseRtfKeyword(FILE *fp)
{
    int ch;
    char fParam = fFalse;
    char fNeg = fFalse;
    int param = 0;
    char *pch;
    char szKeyword[30];
    char szParameter[20];

    szKeyword[0] = '\0';
    szParameter[0] = '\0';
    if ((ch = getc(fp)) == EOF)
        return ecEndOfFile;
    if (!isalpha(ch))           // a control symbol; no delimiter.
    {
        szKeyword[0] = (char) ch;
        szKeyword[1] = '\0';
        return ecTranslateKeyword(szKeyword, 0, fParam);
    }
    for (pch = szKeyword; isalpha(ch); ch = getc(fp))
        *pch++ = (char) ch;
    *pch = '\0';
    if (ch == '-')
    {
        fNeg  = fTrue;
        if ((ch = getc(fp)) == EOF)
            return ecEndOfFile;
    }
    if (isdigit(ch))
    {
        fParam = fTrue;         // a digit after the control means we have a parameter
        for (pch = szParameter; isdigit(ch); ch = getc(fp))
            *pch++ = (char) ch;
        *pch = '\0';
        param = atoi(szParameter);
        if (fNeg)
            param = -param;
        lParam = atol(szParameter);
        if (fNeg)
            param = -param;
    }
    if (ch != ' ')
        ungetc(ch, fp);
    return ecTranslateKeyword(szKeyword, param, fParam);
}

//
// %%Function: ecParseChar
//
// Route the character to the appropriate destination stream.
//

int
ecParseChar(int ch)
{
    if (ris == risBin && --cbBin <= 0)
        ris = risNorm;
    switch (rds)
    {
    case rdsSkip:
        // Toss this character.
        return ecOK;
    case rdsNorm:
        // Output a character. Properties are valid at this point.
        return ecPrintChar(ch);
    default:
        // handle other destinations....
        return ecOK;
    }
}

//
// %%Function: ecPrintChar
//
// Send a character to the output file.
//

int
ecPrintChar(int ch)
{
    // unfortunately, we don't do a whole lot here as far as layout goes...
    //putchar(ch);
    putc (ch, outfp);
    return ecOK;
}

