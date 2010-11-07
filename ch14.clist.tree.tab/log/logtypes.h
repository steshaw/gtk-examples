typedef struct {

   int year;
   int month;
   int day;
   int hours;
   int minutes;
   int seconds;

} typTimeStamp;


typedef struct {

    char *sIp;
    char *sUser;
    char *sDate;
    char *sCmd;
    char *sURL;
    int nResult;
    int nSize;
    typTimeStamp date;

} typHit;

typedef struct {

    char *sURL;
    long nHits;
    long nSize;

} typStat;


typedef struct {

   int year;
   int month;
   int day;

} typDate;


typedef struct {

    long nHits;
    long nSize;
    typDate *date;

} typDateInfo;
