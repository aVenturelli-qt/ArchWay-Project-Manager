#ifndef PATHINFO_H
#define PATHINFO_H
#include <QWidget>
#include <QFileInfo>
#include <QPixmap>
#include <QPainter>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QMessageBox>


/* FUTURE POSSIBLE IMPLEMENNTATIONS
 *
 * 1) swap method within move-constructor to flip two rows on the model with ease
 * 2) implementation of operator== and operator< for comparisons and sort
 *
 *
 *
*/


class PathInfo: QObject
{
    Q_OBJECT

public:
    Q_SIGNAL void pathChanged( QString old_path, QString new_path );
    Q_SIGNAL void validityChanged(bool isValid);
    Q_SIGNAL void openableChanged(bool isOpenable);

    const QPixmap VALID_PIX  { validityPixmap(true)  };
    const QPixmap INVALID_PIX{ validityPixmap(false) };

    const QPixmap NOT_A_FILE_PIX { QPixmap(":/icons/not_existing_icon.svg").scaledToHeight(52, Qt::SmoothTransformation) };
    const QPixmap FILE_PIX       { QPixmap(":/icons/file_icon.svg").scaledToHeight(52, Qt::SmoothTransformation) };
    const QPixmap FOLDER_PIX     { QPixmap(":/icons/folder_icon.svg").scaledToHeight(52, Qt::SmoothTransformation) };

    // constructors
    PathInfo( ) : m_path{ "" } {}
    PathInfo( QString path );
    PathInfo( char* path );

    PathInfo(const PathInfo& other );

    // destructor
    ~PathInfo() {} ;

    // operators overload
    QVariant operator[]( int column );
    PathInfo operator= (const PathInfo& other );

    // getters
    bool openable()     const { return m_isOpenable; }
    bool fileType()     const { return m_isFile; }
    bool pathValidity() const { return m_isValid; }
    QString path()      const { return m_path; }

    //setters
    void changePath ( QString new_path );
    void setOpenable( bool state );
    void checkPathValidity();


    // open file
    void openPath( bool ignoreIsOpenable = false );

    // file info
    QList<QString> absolute_fullPath_fName();
    QString absPath()  const;
    QString fileName() const;
    QString filePath() const;

    // static convinience methods
    static bool pathIsValid( const QString path );
    static int  isFile( const QString path );

    void printInfo();

private:
    QString m_path;
    bool m_isValid{ false };
    bool m_isFile{ false };
    bool m_isOpenable{ true };

    QFileInfo m_file_info;

    void updatePathInfo();

    QPixmap validityPixmap(bool valid);
};





#endif // PATHINFO_H
