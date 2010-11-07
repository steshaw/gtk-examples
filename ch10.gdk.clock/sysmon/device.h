
#define DELTA 1
#define ACTUAL 2

#define MAX_VALUES 200

typedef long typHistoryValue;

typedef struct {
    char            *sName;      /* --- Device name --- */
    int             nType;       /* --- Type - DELTA or ACTUAL --- */
    typHistoryValue nLast;       /* --- Last value (for DELTA) --- */
    typHistoryValue nMax;        /* --- Max value --- */
    typHistoryValue nValues[MAX_VALUES]; /* --- Values saved --- */
} typDevice;


/* 
 * Prototypes.
 */


void NewValue (typDevice *dev, 
        typHistoryValue value,
        int        bInit);
void InitDevice (typDevice *dev, char *name);
typDevice *LookupDevice (char *sName);
void UpdateDevice (char *sDevName, long nValue, int nType, long nMax);
void UpdateExistingDevice (char *sDevName, long nValue);
void GetPacketInfo ();
void GetCPUInfo ();
