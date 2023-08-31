#include "sortproxymodel.h"
#include <QDateTime>

SortProxyModel::SortProxyModel(QObject* parent) : QSortFilterProxyModel(parent)
{
	setDynamicSortFilter(true);
	sort(0, Qt::AscendingOrder);
}

QAbstractItemModel* SortProxyModel::cSourceModel() const
{
	return m_cSourceModel;
}

void SortProxyModel::setSourceModel(QAbstractItemModel* sourceModel)
{
	m_cSourceModel = sourceModel;
	QSortFilterProxyModel::setSourceModel(sourceModel);
}

bool SortProxyModel::lessThan(const QModelIndex& source_left, const QModelIndex& source_right) const
{
	// ͨ����ǰ��ͼ�е�indexλ�û�ȡmodel��ʵ�ʵ�����
	QString creationTime0 = sourceModel()->data(source_left, GcodeModelItem::E_GCodeFileTime).toString();
	QString creationTime1 = sourceModel()->data(source_right, GcodeModelItem::E_GCodeFileTime).toString();

	QDateTime dateTime0 = QDateTime::fromString(creationTime0, "yyyy-MM-dd hh:mm:ss");
	QDateTime dateTime1 = QDateTime::fromString(creationTime1, "yyyy-MM-dd hh:mm:ss");

	return dateTime0 > dateTime1;
}
