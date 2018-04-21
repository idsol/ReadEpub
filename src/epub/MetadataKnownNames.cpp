#include "MetadataKnownNames.h"

/*!
  \class MetadataKnownNames

  Makes known metadata names translatable.
*/

QHash<QString,QString> MetadataKnownNames::__knownNames;

/// \static
QString MetadataKnownNames::query(const QString& name)
{
	initialize();

	if (!name.isEmpty() && __knownNames.contains(name)) {
		return __knownNames[name];
	}
	return QString();
}

/// \static
void MetadataKnownNames::initialize()
{
	if (__knownNames.isEmpty()) {
		__knownNames.insert("title", tr("title")); // ����
		__knownNames.insert("creator", tr("creator")); // ����
		__knownNames.insert("language", tr("language")); // ����
		__knownNames.insert("description", tr("description")); // ���
		__knownNames.insert("publisher", tr("publisher")); // ������
		__knownNames.insert("contributor", tr("contributor")); // ������
		__knownNames.insert("rights", tr("rights")); // ��Ȩ����Ȩ/����Ȩ�ȣ�
		__knownNames.insert("date", tr("date")); // ʱ��
		__knownNames.insert("identifier", tr("identifier")); // ��ʶ��
		__knownNames.insert("subject", tr("subject")); // ��ǩ
		__knownNames.insert("builder", tr("builder")); // ��������
		__knownNames.insert("builder_version", tr("builder_version")); // �������߰汾
		__knownNames.insert("source", tr("source")); // ��Դ
		__knownNames.insert("type", tr("type")); // ����
		__knownNames.insert("coverage", tr("coverage")); // ����
	}
}
