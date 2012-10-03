#define QFAKTURY_VERSION 0.6.7

#define FUNC_TRACE




#ifdef FUNC_TRACE
#define F_TRACE  do {qDebug() << __FUNCTION__ << __LINE__ << __FILE__;} while(0)
#else
#define F_TRACE
#endif
