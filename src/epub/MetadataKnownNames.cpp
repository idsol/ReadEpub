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
		__knownNames.insert("title", tr("title")); // 书名
		__knownNames.insert("creator", tr("creator")); // 作者
		__knownNames.insert("language", tr("language")); // 语言
		__knownNames.insert("description", tr("description")); // 简介
		__knownNames.insert("publisher", tr("publisher")); // 出版者
		__knownNames.insert("contributor", tr("contributor")); // 贡献者
		__knownNames.insert("rights", tr("rights")); // 版权（版权/著作权等）
		__knownNames.insert("date", tr("date")); // 时间
		__knownNames.insert("identifier", tr("identifier")); // 标识码
		__knownNames.insert("subject", tr("subject")); // 标签
		__knownNames.insert("builder", tr("builder")); // 创建工具
		__knownNames.insert("builder_version", tr("builder_version")); // 创建工具版本
		__knownNames.insert("source", tr("source")); // 来源
		__knownNames.insert("type", tr("type")); // 类型
		__knownNames.insert("coverage", tr("coverage")); // 封面
	}
}
