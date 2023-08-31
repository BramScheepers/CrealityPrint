#include "visscenehandler.h"

#include "interface/selectorinterface.h"
#include "interface/visualsceneinterface.h"
#include "interface/modelinterface.h"

namespace creative_kernel
{
	VisSceneHandler::VisSceneHandler(QObject* parent)
		:QObject(parent)
	{
	}
	
	VisSceneHandler::~VisSceneHandler()
	{
	}

	void VisSceneHandler::onHoverEnter(QHoverEvent* event)
	{

	}

	void VisSceneHandler::onHoverLeave(QHoverEvent* event)
	{
		if (shouldMultipleSelect() && m_didSelectModelAtPress == false)
		{
			dismissRectangleSelector();
			requestVisUpdate(false);
		}
	}

	void VisSceneHandler::onHoverMove(QHoverEvent* event)
	{
		if (hoverVis(event->pos()))
			requestVisUpdate(false);
	}

	void VisSceneHandler::onKeyPress(QKeyEvent* event)
	{
		if (event->key() == Qt::Key_Delete)//�û��ڼ����ϰ���Delete��ʱ
		{
			qDebug() << "VisSceneKeyHandler::onKeyPress: delete";
			removeSelectionModels(true);
		}

		if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_Delete))//Ctrl+Delete ɾ������
		{
			qDebug() << "VisSceneKeyHandler::onKeyPress: ctrl + delete";
			removeAllModel(true);
		}

		if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_A))//���û�����Ctrl+A��ʱ
		{
			qDebug() << "VisSceneKeyHandler::onKeyPress: ctrl + A";
			selectAll();
		}

		if (event->modifiers() == Qt::ControlModifier && (event->key() == Qt::Key_V))
		{
			qDebug() << "VisSceneKeyHandler::onKeyPress: ctrl + V";
		}
	}

	void VisSceneHandler::onKeyRelease(QKeyEvent* event)
	{
	}

	void VisSceneHandler::onLeftMouseButtonPress(QMouseEvent* event)
	{
		if (shouldMultipleSelect())
		{
			QPoint p = event->pos();
			m_posOfLeftMousePress = p;
			m_didSelectModelAtPress = didSelectAnyEntity(p);
		}
	}

	void VisSceneHandler::onLeftMouseButtonRelease(QMouseEvent* event)
	{
		if (shouldMultipleSelect() && m_didSelectModelAtPress == false)
		{
			dismissRectangleSelector();
			//��ѡ
			QRect rect = QRect(m_posOfLeftMousePress, event->pos());
			if (rect.size().isNull()) return;
			int width = abs(rect.width()), height = abs(rect.height());
			if (width <= 1 || height <= 1) return;

			selectArea(rect);
			requestVisUpdate();
		}
	}

	void VisSceneHandler::onLeftMouseButtonMove(QMouseEvent* event)
	{
		if (shouldMultipleSelect() && m_didSelectModelAtPress == false)
		{
			//����ѡ���
			QRect rect = QRect(m_posOfLeftMousePress, event->pos());
			if (rect.size().isNull()) return;
			int width = abs(rect.width()), height = abs(rect.height());
			if (width <= 1 || height <= 1) return;

			showRectangleSelector(rect);
			requestVisUpdate();
		}
		
	}

	void VisSceneHandler::onLeftMouseButtonClick(QMouseEvent* event)
	{
		selectVis(event->pos(), event->modifiers() == Qt::KeyboardModifier::ControlModifier);
		requestVisUpdate();
	}
}