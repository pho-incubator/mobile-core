#include "gassetmodel.h"
#include "gassetmodel_p.h"
#include <QRegExp>

Q_LOGGING_CATEGORY(LAssetModel, "GroupsInc.Mvc.AssetModel")

AssetModelPrivate::AssetModelPrivate(AssetModel *q)
    : QObject(0)
    , q_ptr(q)
    , m_qmlPath({ QStringLiteral(""), false })
    , m_assetPath({ QStringLiteral(""), false })
    , m_imageFolderName(QStringLiteral("images"))
    , m_fontFolderName(QStringLiteral("font"))
    , m_translationFolderName(QStringLiteral("translation"))
{
}

const char *AssetModel::NAME = "AssetModel";

AssetModel::AssetModel(MvcFacade *parent)
    : MvcModel(parent)
    , d_ptr(new AssetModelPrivate(this))
{
}

AssetModel::~AssetModel()
{
    delete d_ptr;
}

void AssetModel::setQmlPath(const QString &qmlPath)
{
    Q_D(AssetModel);
    d->m_qmlPath = parsePath(qmlPath);
    emit qmlPathChanged();
}

QString AssetModel::qmlPath() const
{
    Q_D(const AssetModel);
    return getPath(false, d->m_qmlPath);
}

void AssetModel::setAssetPath(const QString &assetPath)
{
    Q_D(AssetModel);
    d->m_assetPath = parsePath(assetPath);
    emit assetPathChanged();
}

QString AssetModel::assetPath() const
{
    Q_D(const AssetModel);
    return getPath(false, d->m_assetPath);
}

void AssetModel::setImageFolderName(const QString &imageFolderName)
{
    Q_D(AssetModel);
    d->m_imageFolderName = imageFolderName;
    emit imageFolderNameChanged();
}

QString AssetModel::imageFolderName() const
{
    Q_D(const AssetModel);
    return d->m_imageFolderName;
}

void AssetModel::setFontFolderName(const QString &fontFolderName)
{
    Q_D(AssetModel);
    d->m_fontFolderName = fontFolderName;
    emit fontFolderNameChanged();
}

QString AssetModel::fontFolderName() const
{
    Q_D(const AssetModel);
    return d->m_fontFolderName;
}

void AssetModel::setTranslationFolderName(const QString &translationFolderName)
{
    Q_D(AssetModel);
    d->m_translationFolderName = translationFolderName;
    emit translationFolderNameChanged();
}

QString AssetModel::translationFolderName() const
{
    Q_D(const AssetModel);
    return d->m_translationFolderName;
}

QString AssetModel::qmlUrl(const QString &qmlName) const
{
    Q_D(const AssetModel);
    return getUrl(d->m_qmlPath, qmlName);
}

QString AssetModel::assetUrl(const QString &assetName) const
{
    Q_D(const AssetModel);
    return getUrl(d->m_assetPath, assetName);
}

QString AssetModel::imageUrl(const QString &imageName) const
{
    Q_D(const AssetModel);
    return getUrl(d->m_assetPath, d->m_imageFolderName, imageName);
}

QString AssetModel::fontUrl(const QString &fontName) const
{
    Q_D(const AssetModel);
    return getUrl(d->m_assetPath, d->m_fontFolderName, fontName);
}

QString AssetModel::translationUrl(const QString &translationName) const
{
    Q_D(const AssetModel);
    return getUrl(d->m_assetPath, d->m_translationFolderName, translationName);
}

QString AssetModel::qmlPath(const QString &qmlName) const
{
    Q_D(const AssetModel);
    return getPath(false, d->m_qmlPath, qmlName);
}

QString AssetModel::assetPath(const QString &assetName) const
{
    Q_D(const AssetModel);
    return getPath(false, d->m_assetPath, assetName);
}

QString AssetModel::imagePath(const QString &imageName) const
{
    Q_D(const AssetModel);
    return getPath(false, d->m_assetPath, d->m_imageFolderName, imageName);
}

QString AssetModel::fontPath(const QString &fontName) const
{
    Q_D(const AssetModel);
    return getPath(false, d->m_assetPath, d->m_translationFolderName, fontName);
}

QString AssetModel::translationPath(const QString &translationName) const
{
    Q_D(const AssetModel);
    return getPath(false, d->m_assetPath, d->m_translationFolderName, translationName);
}

AssetModel::Path AssetModel::parsePath(const QString &path)
{
    AssetModel::Path retval;

    QRegExp regex(QStringLiteral("^(qrc)?:/"));
    if (regex.indexIn(path) > -1) {
        retval.path = QLatin1Char('/') + path.mid(regex.matchedLength());
        retval.isQrc = true;
    } else {
        retval.path = path;
        retval.isQrc = false;
    }

    qCDebug(LAssetModel) << "Path parsed: " << path << "to" << retval.path << "QRC:" << (retval.isQrc ? "YES" : "NO");

    return retval;
}

QString AssetModel::getUrl(AssetModel::Path path, const QString &folderName, const QString &data) const
{
    const QString url = getPath(true, path, folderName, data);
    return url;
}

QString AssetModel::getPath(bool isQrc, AssetModel::Path path, const QString &folderName, const QString &data) const
{
    QString realPath;
    if (path.isQrc)
        realPath += QLatin1String(isQrc ? "qrc:" : ":");

    realPath += path.path;

    if (!folderName.isEmpty() && !data.isEmpty()) {
        realPath = QString::fromLatin1("%1/%2/%3").arg(realPath, folderName, data);
    } else if (!folderName.isEmpty()) {
        realPath = QString::fromLatin1("%1/%2").arg(realPath, folderName);
    }

    return static_cast<const QString>(realPath);
}

void AssetModel::init()
{
}

const char *AssetModel::name()
{
    return NAME;
}

void AssetModel::apply(const QVariantMap &config)
{
    Q_UNUSED(config);
}
