#ifndef ASSETMODEL_H
#define ASSETMODEL_H

#include <QObject>
#include <QLoggingCategory>
#include "gmvcmodel.h"
#include <qqml.h>

Q_DECLARE_LOGGING_CATEGORY(LAssetModel)

class AssetModelPrivate;

class Q_MVC_EXPORT AssetModel : public MvcModel
{
    Q_OBJECT
    Q_PROPERTY(QString qmlPath READ qmlPath WRITE setQmlPath NOTIFY qmlPathChanged)
    Q_PROPERTY(QString assetPath READ assetPath WRITE setAssetPath NOTIFY assetPathChanged)
    Q_PROPERTY(QString imageFolderName READ imageFolderName WRITE setImageFolderName NOTIFY imageFolderNameChanged)
    Q_PROPERTY(QString fontFolderName READ fontFolderName WRITE setFontFolderName NOTIFY fontFolderNameChanged)
    Q_PROPERTY(QString translationFolderName READ translationFolderName WRITE setTranslationFolderName NOTIFY translationFolderNameChanged)
public:
    static const char *NAME;

    typedef struct {
        QString path;
        bool isQrc;
    } Path;

    explicit AssetModel(MvcFacade *parent = 0);
    ~AssetModel();

    void setQmlPath(const QString &qmlPath);
    QString qmlPath() const;

    void setAssetPath(const QString &assetPath);
    QString assetPath() const;

    void setImageFolderName(const QString &imageFolderName);
    QString imageFolderName() const;

    void setFontFolderName(const QString &fontFolderName);
    QString fontFolderName() const;

    void setTranslationFolderName(const QString &translationFolderName);
    QString translationFolderName() const;

    Q_INVOKABLE QString qmlUrl(const QString &qmlName) const;
    Q_INVOKABLE QString assetUrl(const QString &assetName) const;
    Q_INVOKABLE QString imageUrl(const QString &imageName) const;
    Q_INVOKABLE QString fontUrl(const QString &fontName) const;
    Q_INVOKABLE QString translationUrl(const QString &translationName) const;

    Q_INVOKABLE QString qmlPath(const QString &qmlName) const;
    Q_INVOKABLE QString assetPath(const QString &assetName) const;
    Q_INVOKABLE QString imagePath(const QString &imageName) const;
    Q_INVOKABLE QString fontPath(const QString &fontName) const;
    Q_INVOKABLE QString translationPath(const QString &translationName) const;

    void init();
    const char *name();
    void apply(const QVariantMap &config);

Q_SIGNALS:
    void qmlPathChanged();
    void assetPathChanged();
    void imageFolderNameChanged();
    void fontFolderNameChanged();
    void translationFolderNameChanged();

private:
    Q_DECLARE_PRIVATE(AssetModel)
    AssetModelPrivate *d_ptr;

    AssetModel::Path parsePath(const QString &path);
    QString getUrl(AssetModel::Path path, const QString &folderName, const QString &data = QString()) const;
    QString getPath(bool isQrc, AssetModel::Path path, const QString &folderName = QString(), const QString &data = QString()) const;
};

QML_DECLARE_TYPE(AssetModel)

#endif // ASSET_MODEL_H
