#ifndef GAPIMODEL_FORMAT_JSON_P_H
#define GAPIMODEL_FORMAT_JSON_P_H

#include "gapimodel_format_p.h"

class QJsonArray;
class QJsonObject;

class ApiModelFormatJsonPrivate : public ApiModelFormatPrivate
{
    Q_OBJECT
public:
    ApiModelFormatJsonPrivate();

    virtual void prepareRequest(QNetworkRequest &request, ApiModel::ApiMethod method, const QByteArray &requestData);
    virtual QVariant parse(const QByteArray &data, ApiFormatError *error = 0);
    virtual QByteArray assemble(const QVariant &data, ApiFormatError *error = 0);
};

#endif // GAPIMODEL_FORMAT_JSON_P_H
