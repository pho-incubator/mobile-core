#ifndef ASSETMODEL_P_H
#define ASSETMODEL_P_H

#include <QObject>
#include "gmvc_global.h"
#include "gassetmodel.h"

class Q_MVC_EXPORT AssetModelPrivate : public QObject
{
    Q_OBJECT
public:
    AssetModelPrivate(AssetModel *q);

protected:
    Q_DECLARE_PUBLIC(AssetModel)
    AssetModel *q_ptr;

    AssetModel::Path m_qmlPath;
    AssetModel::Path m_assetPath;
    QString m_imageFolderName;
    QString m_fontFolderName;
    QString m_translationFolderName;


};

#endif // ASSETMODEL_P_H
