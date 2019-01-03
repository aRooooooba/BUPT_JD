#ifndef VARIETIES
#define VARIETIES

#include <qstring.h>
#include <qvector.h>
#include "user_class.h"
#include "date_class.h"
#include "book_class.h"
#include "cloth_class.h"
#include "electronic_class.h"

extern QString usersFileAddress;
extern QString dateFileAddress;
extern QString booksFileAddress;
extern QString clothesFileAddress;
extern QString electronicsFileAddress;
extern QString deleteBtnURL;
extern QVector <user_CLASS> users;
extern QVector <book_CLASS> books;
extern QVector <cloth_CLASS> clothes;
extern QVector <electronic_CLASS> electronics;
extern date_CLASS date;
extern user_CLASS * thisUser;
extern bool timeFlag;

bool loadUsers();
void saveUsers();
bool loadBooks();
bool loadClothes();
bool loadElectronics();
bool findItem(QString id, book_CLASS * &thisBook, cloth_CLASS * &thisCloth, electronic_CLASS * &thisElectronic);

#endif // VARIETIES

