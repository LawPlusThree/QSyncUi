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

