#include "xmlprocesser.h"

Bucket XmlProcesser::processXml(const QString &xmlString)
{
    Bucket bucket;
    QDomDocument dom;
    dom.setContent(xmlString);
    QDomElement ListBucketResultEle = dom.documentElement();
    QString ListBucketResult = ListBucketResultEle.tagName();
    qDebug() << ListBucketResult;

    QDomNodeList nodeList = ListBucketResultEle.childNodes();
    for (int i = 0; i < nodeList.count(); ++i) {
        QDomNode node = nodeList.at(i);
        if (node.isElement()) {
            QDomElement element = node.toElement();
            QString tagName = element.tagName();
            QString content = element.text();
            if(tagName == "Name"){
                bucket.name=content;
            }else if(tagName == "EncodingType"){
                bucket.encodingType=content;
            }else if(tagName == "Prefix"){
                bucket.prefix=content;
            }else if(tagName == "Marker"){
                bucket.marker=content;
            }else if(tagName == "MaxKeys"){
                bucket.maxKeys=content.toInt();
            }else if(tagName == "Delimiter"){
                bucket.delimiter=content;
            }else if(tagName == "IsTruncated"){
                bucket.isTruncated=(content.toStdString()=="true"?true:false);
            }else if(tagName == "NextMarker"){
                bucket.nextMarker=content;
            }

            if (tagName == "CommonPrefixes") {
                QDomNodeList commonPrefixesList = element.childNodes();
                for (int j = 0; j < commonPrefixesList.count(); ++j) {
                    QDomNode commonPrefixNode = commonPrefixesList.at(j);
                    if (commonPrefixNode.isElement()) {
                        QDomElement commonPrefixElement = commonPrefixNode.toElement();
                        //QString commonPrefixTagName = commonPrefixElement.tagName();
                        QString commonPrefixContent = commonPrefixElement.text();
                        bucket.commonPrefixes.push_back(commonPrefixContent);
                    }
                }
            } else if (tagName == "Contents") {
                QDomNodeList contentsList = element.childNodes();
                Content content;
                for (int j = 0; j < contentsList.count(); ++j) {
                    QDomNode contentNode = contentsList.at(j);
                    if (contentNode.isElement()) {
                        QDomElement contentElement = contentNode.toElement();
                        QString contentTagName = contentElement.tagName();
                        QString contentContent = contentElement.text();
                        if(contentTagName=="Key"){
                            content.key=contentContent;
                        }
                        else if(contentTagName=="LastModified"){
                            content.lastModified=QDateTime::fromString(contentContent,"yyyy-MM-ddThh:mm:ss.zzzZ");
                        }
                        else if(contentTagName=="ETag"){
                            content.eTag=contentContent;
                        }
                        else if(contentTagName=="Size"){
                            content.size=contentContent.toInt();
                        }
                        else if(contentTagName=="Owner"){
                            QDomNodeList ownerList = contentElement.childNodes();
                            for (int k = 0; k < ownerList.count(); ++k) {
                                QDomNode ownerNode = ownerList.at(k);
                                if (ownerNode.isElement()) {
                                    QDomElement ownerElement = ownerNode.toElement();
                                    QString ownerTagName = ownerElement.tagName();
                                    QString ownerContent = ownerElement.text();
                                    if(ownerTagName=="ID"){
                                        content.owner.id=ownerContent;
                                    }
                                    else if(ownerTagName=="DisplayName"){
                                        content.owner.displayName=ownerContent;
                                    }
                                }
                            }
                        }
                        else if(contentTagName=="StorageClass"){
                            content.storageClass=contentContent;
                        }
                        else if(contentTagName=="StorageTier"){
                            content.storageTier=contentContent;
                        }
                        else if(contentTagName=="RestoreStatus"){
                            content.restoreStatus=contentContent;
                        }
                    }
                }
                bucket.contents.push_back(content);
            }
        }
    }
    return bucket;
}





VersionResult HistoryXMLProcesser::processXml(const QString &xmlString)
{
    VersionResult versionResult;
    QDomDocument dom;
    dom.setContent(xmlString);
    QDomElement ListVersionsResultEle = dom.documentElement();
    QString ListVersionsResult = ListVersionsResultEle.tagName();
    qDebug() << ListVersionsResult;

    QDomNodeList nodeList = ListVersionsResultEle.childNodes();
    for (int i = 0; i < nodeList.count(); ++i) {
        QDomNode node = nodeList.at(i);
        if (node.isElement()) {
            QDomElement element = node.toElement();
            QString tagName = element.tagName();
            QString content = element.text();
            if(tagName == "Name"){
                versionResult.name=content;
            }else if(tagName == "EncodingType"){
                versionResult.encodingType=content;
            }else if(tagName == "KeyMarker"){
                versionResult.keyMarker=content;
            }else if(tagName == "VersionIdMarker"){
                versionResult.versionIdMarker=content;
            }

            if (tagName == "Version") {
                QDomNodeList versionList = element.childNodes();
                Version version;
                for (int j = 0; j < versionList.count(); ++j) {
                    QDomNode versionNode = versionList.at(j);
                    if (versionNode.isElement()) {
                        QDomElement versionElement = versionNode.toElement();
                        QString versionTagName = versionElement.tagName();
                        QString versionContent = versionElement.text();
                        if(versionTagName=="Key"){
                            version.key=versionContent;
                        }
                        else if(versionTagName=="VersionId"){
                            version.versionId=versionContent;
                        }
                        else if(versionTagName=="IsLatest"){
                            version.isLatest=(versionContent.toStdString()=="true"?true:false);
                        }
                        else if(versionTagName=="LastModified"){
                            version.lastModified= QDateTime::fromString(versionContent, Qt::RFC2822Date);
                        }
                        else if(versionTagName=="ETag"){
                            version.eTag=versionContent;
                        }
                        else if(versionTagName=="Size"){
                            version.size=versionContent.toInt();
                        }
                        else if(versionTagName=="Owner"){
                            QDomNodeList ownerList = versionElement.childNodes();
                            for (int k = 0; k < ownerList.count(); ++k) {
                                QDomNode ownerNode = ownerList.at(k);
                                if (ownerNode.isElement()) {
                                    QDomElement ownerElement = ownerNode.toElement();
                                    QString ownerTagName = ownerElement.tagName();
                                    QString ownerContent = ownerElement.text();
                                    if(ownerTagName=="ID"){
                                        version.owner.id=ownerContent;
                                    }
                                    else if(ownerTagName=="DisplayName"){
                                        version.owner.displayName=ownerContent;
                                    }
                                }
                            }
                        }
                        else if(versionTagName=="StorageClass"){
                            version.storageClass=versionContent;
                        }
                        else if(versionTagName=="StorageTier"){
                            version.storageTier=versionContent;
                        }
                    }
                }
                versionResult.versions.push_back(version);
            }
        }
    }
    return versionResult;
}
