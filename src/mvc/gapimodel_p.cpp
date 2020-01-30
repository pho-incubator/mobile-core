#include "gapimodel_p.h"
#include "gapimodel.h"
#include "gapimodel_format_json_p.h"
#include "gapimodel_format_query_p.h"
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QSslConfiguration>
#include <QUrlQuery>

ApiModelPrivate::ApiModelPrivate(ApiModel *q)
    : QObject(0)
    , q_ptr(q)
    , m_networkAccessManager(new QNetworkAccessManager())
    , m_currentReply(0)
    , m_cookieJar(new QNetworkCookieJar())
    , m_queryFormatType(ApiModel::FORMAT_JSON)
    , m_responseFormatType(ApiModel::FORMAT_JSON)
    , m_queryFormat(ApiModelFormatPrivate::factory(ApiModel::FORMAT_JSON))
    , m_responseFormat(ApiModelFormatPrivate::factory(ApiModel::FORMAT_JSON))
    , m_responseValidatorCallback(0)
    , m_responseHeaderCallback(0)
    , m_requestCount(0)
    , m_restrictMode(false)
{
}

ApiModelPrivate::~ApiModelPrivate()
{
    if (m_networkAccessManager) {
        m_networkAccessManager->deleteLater();
        m_networkAccessManager = 0;
    }

    delete m_responseFormat;
    delete m_queryFormat;
}

void ApiModelPrivate::init()
{
    m_networkAccessManager->setCookieJar(m_cookieJar);
    connect(m_networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(reply(QNetworkReply*)));
}

quint32 ApiModelPrivate::request(ApiModel::ApiMethod method, const QUrl &url, const QVariant &data, const QString &identifier)
{
    m_requestCount++;
    Q_Q(ApiModel);

    if (!q->availableForRequest()) {
        emit q->error(ApiModel::ShouldWaitUntilCurrentProcessIsCompleted, tr("Should wait until the current process is completed."), identifier, m_requestCount);
        return m_requestCount;
    }

    QNetworkRequest request;
    ApiFormatError error;
    QNetworkReply *reply;

    request.setUrl(url);
    QByteArray requestData = m_queryFormat->assemble(data, &error);
    m_queryFormat->prepareRequest(request, method, requestData);

    QString mIdentifier = identifier;
    if (mIdentifier.isEmpty())
        mIdentifier = QStringLiteral("Request#%1").arg(m_requestCount);

    QUrl finalUrl = static_cast<QUrl>(request.url());

    QUrlQuery finalUrlQuery;
    finalUrlQuery.setQuery(finalUrl.query());
    finalUrlQuery.addQueryItem(QLatin1String("_i"), mIdentifier);
    finalUrlQuery.addQueryItem(QLatin1String("_d"), QString::number(m_requestCount));

    finalUrl.setQuery(finalUrlQuery);
    request.setUrl(finalUrl);

    qCDebug(LApiModel) << "Final URL" << finalUrl;

    switch (method) {
    case ApiModel::METHOD_POST:
        reply = m_networkAccessManager->post(request, requestData);
        qCDebug(LApiModel) << "Post Data" << requestData;
        break;
    case ApiModel::METHOD_PUT:
        reply = m_networkAccessManager->put(request, requestData);
        break;
    case ApiModel::METHOD_GET:
        reply = m_networkAccessManager->get(request);
        break;
    // case ApiModel::METHOD_DELETE: altough not necessary
    default:
        reply = m_networkAccessManager->deleteResource(request);
    }

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(error(QNetworkReply::NetworkError)));
    m_currentReply = reply;

    return m_requestCount;
}

void ApiModelPrivate::reply(QNetworkReply *reply)
{
    Q_Q(ApiModel);

    QByteArray response = reply->readAll();
    qCDebug(LApiModel) << reply->url()
                       << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute)
                       << reply->header(QNetworkRequest::ContentTypeHeader)
                       << response
                       << reply->error()
                       << reply->errorString();

    QUrlQuery query;
    query.setQuery(reply->url().query());

    QString identifier;
    if (query.hasQueryItem(QLatin1String("_i")))
        identifier = query.queryItemValue(QLatin1String("_i"));

    bool ok = false;
    quint64 requestId = 0;
    if (query.hasQueryItem(QLatin1String("_d"))) {
        requestId = query.queryItemValue(QLatin1String("_d")).toUInt(&ok);
        if (!ok)
            requestId = 0;

        qCDebug(LApiModel) << "Request Id: " << requestId << query.queryItemValue(QLatin1String("_d")) << query.toString();
    } else {
        qCDebug(LApiModel) << "Request Id: " << requestId << query.queryItemValue(QLatin1String("_d")) << query.toString();
    }

    if (reply->error() == QNetworkReply::NoError) {
        if (m_responseHeaderCallback)
            m_responseHeaderCallback(reply, q, identifier);

        ApiFormatError error;
        QVariant data = m_responseFormat->parse(response.trimmed(), &error);

        bool gotError = true;
        if (error.error != ApiFormatError::NoError) {
            emit q->error(ApiModel::UnexpectedServerResponse, tr("Unexpected server response."), identifier, requestId);
        } else {
            if (m_responseValidatorCallback && !m_responseValidatorCallback(data, q, &gotError))
                if (gotError)
                    emit q->error(ApiModel::UnexpectedServerResponse, tr("Unexpected server response."), identifier, requestId);

            emit q->response(reply->url(), data, identifier, requestId);
        }
    } else {
        emit q->error(ApiModel::CouldNotConnectToHost, tr("Could not connect to server."), identifier, requestId);
    }

    if (m_currentReply == reply)
        m_currentReply = 0;

    reply->deleteLater();
}

void ApiModelPrivate::error(QNetworkReply::NetworkError error)
{
    Q_Q(ApiModel);
    Q_UNUSED(error);

    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    QUrlQuery query;
    query.setQuery(reply->url().query());

    QString identifier;
    if (query.hasQueryItem(QLatin1String("_i")))
        identifier = query.queryItemValue(QLatin1String("_i"));

    bool ok = false;
    quint64 requestId = 0;
    if (query.hasQueryItem(QLatin1String("_d"))) {
        requestId = query.queryItemValue(QLatin1String("_d")).toUInt(&ok);
        if (!ok)
            requestId = 0;
    }

    emit q->error(ApiModel::CouldNotConnectToHost, tr("Could not connect to server."), identifier, requestId);

    if (reply == m_currentReply)
        m_currentReply = 0;

    reply->deleteLater();
}
