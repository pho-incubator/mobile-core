#ifndef GAPIMODEL_FORMAT_P_H
#define GAPIMODEL_FORMAT_P_H

#include <QObject>
#include "gapimodel.h"

class QNetworkRequest;
class ApiModelFormatJsonPrivate;
class ApiModelFormatQueryPrivate;

class ApiModelFormatPrivate : public QObject
{
    Q_OBJECT
public:
    ApiModelFormatPrivate();

    virtual void prepareRequest(QNetworkRequest &request, ApiModel::ApiMethod method, const QByteArray &requestData) = 0;
    virtual QVariant parse(const QByteArray &data, ApiFormatError *error = 0) = 0;
    virtual QByteArray assemble(const QVariant &data, ApiFormatError *error = 0) = 0;

    static ApiModelFormatPrivate *factory(ApiModel::ApiDataFormat format);
};

#endif // GAPIMODEL_FORMAT_P_H
