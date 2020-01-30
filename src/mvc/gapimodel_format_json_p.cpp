#include "gapimodel_format_json_p.h"
#include "gmvcfacade.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QNetworkRequest>
#include "gapimodel.h"

ApiModelFormatJsonPrivate::ApiModelFormatJsonPrivate()
    : ApiModelFormatPrivate()
{
}

void ApiModelFormatJsonPrivate::prepareRequest(QNetworkRequest &request, ApiModel::ApiMethod method, const QByteArray &requestData)
{
    Q_UNUSED(requestData);
    if (method == ApiModel::METHOD_POST || method == ApiModel::METHOD_PUT)
        request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json; charset=UTF-8"));
}

QVariant ApiModelFormatJsonPrivate::parse(const QByteArray &data, ApiFormatError *error)
{
    QJsonParseError parserError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parserError);
    QVariant ret;

    if (parserError.error != QJsonParseError::NoError) {
        qCCritical(LApiModel) << "Got JSON Error: " << parserError.errorString();
        if (error)
            error->error = ApiFormatError::ParseError;

    } else if (!doc.isObject() && !doc.isArray()) {
        qCCritical(LApiModel) << "Invalid response.";
        if (error)
            error->error = ApiFormatError::ParseError;
    } else {
        if (doc.isObject()) {
            ret.setValue(doc.object().toVariantMap());
        } else if (doc.isArray()) {
            ret.setValue(doc.array().toVariantList());
        } else {
            qCCritical(LApiModel) << "Empty or NULL response. There is nothing to parse.";
            if (error)
                error->error = ApiFormatError::ParseError;
        }
    }

    return ret;
}

QByteArray ApiModelFormatJsonPrivate::assemble(const QVariant &data, ApiFormatError *error)
{
    Q_UNUSED(error);

    QJsonDocument doc;
    if (data.type() == QVariant::List) {
        QJsonArray array;
        array.fromVariantList(data.toList());
        doc.setArray(array);
    } else if (data.type() == QVariant::Map) {
        QJsonObject object;
        object.fromVariantMap(data.toMap());
        doc.setObject(object);
    } else {
        qCWarning(LApiModel) << "Data should be Map or List for JSON." << data;
    }

    return doc.toBinaryData();
}
