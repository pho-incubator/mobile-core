#ifndef GAPIMODEL_FORMAT_QUERY_P_H
#define GAPIMODEL_FORMAT_QUERY_P_H

#include "gapimodel_format_p.h"

class QUrlQuery;

class ApiModelFormatQueryPrivate : public ApiModelFormatPrivate
{
    Q_OBJECT
public:
    ApiModelFormatQueryPrivate();

    void prepareRequest(QNetworkRequest &request, ApiModel::ApiMethod method, const QByteArray &requestData);
    QVariant parse(const QByteArray &data, ApiFormatError *error = 0);
    QByteArray assemble(const QVariant &data, ApiFormatError *error = 0);

private:
    void assambleList(QUrlQuery &query, const QVariantList &data, const QString &previousName = QString(), int depth = 0);
    void assambleMap(QUrlQuery &query, const QVariantMap &data, const QString &previousName = QString(), int depth = 0);
    void assembleByValue(const QString &name, QUrlQuery &query, const QVariant &data, int depth = 0);
};

#endif // GAPIMODEL_FORMAT_QUERY_P_H
