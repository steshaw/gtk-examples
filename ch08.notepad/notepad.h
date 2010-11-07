
int ConvertRTF (char *szFilename, char *szOutFile);

void ShowMessage (char *szTitle, char *szMessage);
GtkWidget *GetTextWidget ();
char *GetText ();
void menu_New ();
void menu_ImportRTF ();
void menu_Find ();
void menu_Open ();
void menu_Save ();
void menu_SaveAs ();
void menu_Cut ();
void menu_Copy ();
void menu_Paste ();
void menu_Quit ();
void TextCut ();
void TextCopy ();
void TextPaste ();
void menu_About ();
void ShowAbout ();
void ClearFile ();
void LoadFile (char *sFilename);
void ImportFile (char *sFilename);
void SaveFile (char *sFilename);
//
// --- Function prototypes
//
void CreateToolbar (GtkWidget *vbox_main);
void SetToolbarButton (char *szButton, int nState);
void SelectMenu (GtkWidget *widget, gpointer data);
void DeSelectMenu (GtkWidget *widget, gpointer data);
void SetMenuButton (char *szButton, int nState) ;
GtkWidget *CreateWidgetFromXpm (GtkWidget *window, gchar **xpm_data);
GtkWidget *CreateMenuItem (GtkWidget *menu, 
                           char *szName, 
                           char *szAccel,
                           char *szTip, 
                           GtkSignalFunc func,
                           gpointer data);
GtkWidget *CreateMenuCheck (GtkWidget *menu, 
                            char *szName, 
                            GtkSignalFunc func, 
                            gpointer data);
GtkWidget *CreateSubMenu (GtkWidget *menubar, char *szName);
GtkWidget *CreateBarSubMenu (GtkWidget *menu, char *szName);
void CreateMenu (GtkWidget *window, GtkWidget *vbox_main);
void CreateText (GtkWidget *window, GtkWidget *container);
void GetFilename (char *sTitle, void (*func) (gchar *));
//
void UpdateProgress (long pos, long len);
void StartProgress ();
void EndProgress ();
