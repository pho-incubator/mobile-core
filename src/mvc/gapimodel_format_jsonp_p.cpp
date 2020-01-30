#include "gapimodel_format_jsonp_p.h"
#include "gmvcfacade.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonParseError>
#include <QNetworkRequest>
#include "gapimodel.h"
#include <QRegExp>

ApiModelFormatJsonPPrivate::ApiModelFormatJsonPPrivate()
    : ApiModelFormatJsonPrivate()
{
}

QVariant ApiModelFormatJsonPPrivate::parse(const QByteArray &data, ApiFormatError *error)
{
    QString response = QString::fromUtf8(data);
    QRegExp regex(QLatin1String("^[a-zA-Z0-9_]*\\("));

    if (regex.indexIn(response) != -1) {
        response = response.mid(regex.matchedLength());
        if (response.endsWith(')'))
            response = response.mid(0, response.length() - 1);

        return ApiModelFormatJsonPrivate::parse(response.toUtf8(), error);
    }

    return ApiModelFormatJsonPrivate::parse(data, error);
}
