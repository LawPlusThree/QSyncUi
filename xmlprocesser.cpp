#include "xmlprocesser.h"

void XmlProcesser::processXml(const QString &xmlString)
{
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
            qDebug() << "tagName:" + tagName << "content:" + content;

            if (tagName == "CommonPrefixes") {
                QDomNodeList commonPrefixesList = element.childNodes();
                for (int j = 0; j < commonPrefixesList.count(); ++j) {
                    QDomNode commonPrefixNode = commonPrefixesList.at(j);
                    if (commonPrefixNode.isElement()) {
                        QDomElement commonPrefixElement = commonPrefixNode.toElement();
                        QString commonPrefixTagName = commonPrefixElement.tagName();
                        QString commonPrefixContent = commonPrefixElement.text();
                        qDebug() << "commonPrefixTagName:" + commonPrefixTagName << "commonPrefixContent:" + commonPrefixContent;
                    }
                }
            } else if (tagName == "Contents") {
                QDomNodeList contentsList = element.childNodes();
                for (int j = 0; j < contentsList.count(); ++j) {
                    QDomNode contentNode = contentsList.at(j);
                    if (contentNode.isElement()) {
                        QDomElement contentElement = contentNode.toElement();
                        QString contentTagName = contentElement.tagName();
                        QString contentContent = contentElement.text();
                        qDebug() << "contentTagName:" + contentTagName << "contentContent:" + contentContent;
                    }
                }
            }
        }
    }
}

