#include "gapimodel_format_query_p.h"
#include <QUrlQuery>
#include "gmvcfacade.h"
#include "gapimodel.h"
#include <QFile>

ApiModelFormatQueryPrivate::ApiModelFormatQueryPrivate()
    : ApiModelFormatPrivate()
{
}

void ApiModelFormatQueryPrivate::prepareRequest(QNetworkRequest &request, ApiModel::ApiMethod method, const QByteArray &requestData)
{
    if (method == ApiModel::METHOD_POST || method == ApiModel::METHOD_PUT)
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded; charset=UTF-8"));
    else {
        QUrl url = static_cast<QUrl>(request.url());
        url.setQuery(QString::fromUtf8(requestData));

        qCDebug(LApiModel) << "Injected query for Url" << url;
        request.setUrl(url);
    }
}

QVariant ApiModelFormatQueryPrivate::parse(const QByteArray &data, ApiFormatError *error)
{
    Q_UNUSED(data);
    Q_UNUSED(error);
    qCWarning(LApiModel) << "ApiModelFormatQueryPrivate::parse() Not yet implemented.";

    return QVariant();
}

QByteArray ApiModelFormatQueryPrivate::assemble(const QVariant &data, ApiFormatError *error)
{
    Q_UNUSED(error);

    QUrlQuery query;
    if (data.type() == QVariant::List)
        assambleList(query, data.toList());
    else if (data.type() == QVariant::Map)
        assambleMap(query, data.toMap());
    else
        qCWarning(LApiModel) << "Data should be List or Map." << data;

    return query.toString(QUrl::FullyEncoded).toUtf8();
}

void ApiModelFormatQueryPrivate::assembleByValue(const QString &name, QUrlQuery &query, const QVariant &data, int depth)
{
    QString value;
    if (data.type() == QVariant::Int)
        value = QString::number(data.toInt());
    else if (data.type() == QVariant::UInt)
        value = QString::number(data.toUInt());
    else if (data.type() == QVariant::LongLong)
        value = QString::number(data.toLongLong());
    else if (data.type() == QVariant::Double)
        value = QString::number(data.toDouble());
    else if (data.type() == QVariant::Bool)
        value = QLatin1String(data.toBool() ? "1" : "0");
    else if (data.type() == QVariant::String) {
        value = data.toString();
        if (value.mid(0, 5) == QLatin1String("@path")) {
            bool convertBase64 = false;
            QString path;

            if (value.mid(5, 8) == QLatin1String(":base64=")) {
                path = value.mid(13);
                convertBase64 = true;
            } else if (value.mid(5, 1) == QLatin1String("=")) {
                path = value.mid(6);
            } else {
                qCWarning(LApiModel) << "Invalid Path syntax." << value;
            }

            if (!path.isEmpty()) {
                if (path.startsWith(QLatin1String("file://")))
                    path = path.mid(7);

                qCDebug(LApiModel) << "Path converting to file:" << path;
                QFile file(path);

                if (file.open(QFile::ReadOnly)) {
                    value = QLatin1String(file.readAll().toBase64());
                    file.close();

                    int fileSize = value.size();
                    qCDebug(LApiModel) << "Adding file size" << fileSize;

                    query.addQueryItem(QString::fromLatin1("_fs[%1]").arg(name), QString::number(fileSize));
                } else {
                    qCWarning(LApiModel) << "File could not opened or could not read." << path;
                }
            }
        }

    } else if (data.type() == QVariant::Map) {
        // qCDebug(LApiModel) << "Data is map. Moving to deep conversation";
        assambleMap(query, data.toMap(), name, depth + 1);
        return;
    } else if (data.type() == QVariant::List) {
        // qCDebug(LApiModel) << "Data is list. Moving to deep conversation";
        assambleList(query, data.toList(), name, depth + 1);
        return;
    } else {
        qDebug(LApiModel) << "Unhandled Variant Type" << data.type() << data;
    }

    // qCDebug(LApiModel) << "Adding query with name" << name;
    query.addQueryItem(name, value);
}

void ApiModelFormatQueryPrivate::assambleList(QUrlQuery &query, const QVariantList &data, const QString &previousName, int depth)
{
    if (depth > MAX_DEPTH_LEVEL || data.isEmpty()) {
        qCWarning(LApiModel) << "Max recursive level excessed or data is empty. Depth:" << depth << "Data Empty:" << data.isEmpty();
        return;
    }

    int i = 0;
    int len = data.size();
    QString name;

    for (; i < len; i++) {
        if (!previousName.isEmpty())
            name = QString::fromUtf8("%1[%2]").arg(previousName).arg(QString::number(i));
        else
            name = QString::number(i);

        assembleByValue(name, query, data.at(i), depth);
    }
}

void ApiModelFormatQueryPrivate::assambleMap(QUrlQuery &query, const QVariantMap &d, const QString &previousName, int depth)
{
    if (depth > MAX_DEPTH_LEVEL || d.isEmpty()) {
        qCWarning(LApiModel) << "Max recursive level excessed or data is empty. Depth:" << depth << "Data Empty:" << d.isEmpty();
        return;
    }

    QVariantMap data = static_cast<QVariantMap>(d);
    QString name;

    for (QVariantMap::iterator it = data.begin(); it != data.end(); ++it) {
        if (!previousName.isEmpty())
            name = QString::fromUtf8("%1[%2]").arg(previousName).arg(it.key());
        else
            name = it.key();

        assembleByValue(name, query, it.value(), depth);
    }
}
