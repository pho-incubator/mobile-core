#ifndef GAPIMODEL_FORMAT_JSONP_P_H
#define GAPIMODEL_FORMAT_JSONP_P_H

#include "gapimodel_format_json_p.h"

class QJsonArray;
class QJsonObject;

class ApiModelFormatJsonPPrivate : public ApiModelFormatJsonPrivate
{
    Q_OBJECT
public:
    ApiModelFormatJsonPPrivate();
    QVariant parse(const QByteArray &data, ApiFormatError *error = 0);
};

#endif // GAPIMODEL_FORMAT_JSON_P_H
