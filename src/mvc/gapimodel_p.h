#ifndef GAPIMODEL_P_H
#define GAPIMODEL_P_H

#include <QObject>
#include <QNetworkReply>
#include "gapimodel.h"

class QNetworkAccessManager;
class QUrl;
class QJsonArray;
class QJsonObject;
class QNetworkCookieJar;
class ApiModelFormatPrivate;

class ApiModelPrivate : public QObject
{
    Q_OBJECT
public:
    explicit ApiModelPrivate(ApiModel *q);
    ~ApiModelPrivate();

public:
    void init();
    quint32 request(ApiModel::ApiMethod method, const QUrl &url, const QVariant &data = QVariant(), const QString &identifier = QString());

private Q_SLOTS:
    void reply(QNetworkReply *reply);
    void error(QNetworkReply::NetworkError);

private:
    Q_DECLARE_PUBLIC(ApiModel)
    ApiModel *q_ptr;

    QNetworkAccessManager *m_networkAccessManager;
    QNetworkReply *m_currentReply;
    QNetworkCookieJar *m_cookieJar;

    QUrl m_baseUrl;

    ApiModel::ApiDataFormat m_queryFormatType;
    ApiModel::ApiDataFormat m_responseFormatType;

    ApiModelFormatPrivate *m_queryFormat;
    ApiModelFormatPrivate *m_responseFormat;

    ApiModel::ApiResponseValidatorCallback m_responseValidatorCallback;
    ApiModel::ApiResponseHeaderCallback m_responseHeaderCallback;

    QVariantMap m_routes;

    quint32 m_requestCount;
    bool m_restrictMode;
};

#endif // GAPIMODEL_P_H
