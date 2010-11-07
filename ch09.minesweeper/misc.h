
GtkWidget *CreateWidgetFromXpm (GtkWidget *widget, gchar **xpm_data);

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

GtkWidget *CreateMenuRadio (GtkWidget *menu, 
                            char *szName, 
                            GSList **group,
                            GtkSignalFunc func, 
                            gpointer data);
