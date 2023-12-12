#include "translatemode.h"
#include "operation/translateop.h"

#include "interface/visualsceneinterface.h"
#include "interface/selectorinterface.h"
#include "interface/commandinterface.h"
#include "interface/uiinterface.h"
#include "kernel/kernelui.h"

namespace creative_kernel
{
    TranslateMode::TranslateMode(QObject* parent)
        :ToolCommand(parent)
        , m_translateOp(nullptr)
    {
        orderindex = 0;
        m_name = tr("Move") + ": M";

        m_source = "qrc:/kernel/qml/MovePanel.qml";
        addUIVisualTracer(this);
    }

    bool TranslateMode::checkSelect()
    {
        return  selected();
    }

    TranslateMode::~TranslateMode()
    {
    }

    void TranslateMode::setMessage(bool isRemove)
    {
        m_translateOp->setMessage(isRemove);
    }

    bool TranslateMode::message()
    {
        if (m_translateOp->getMessage())
        {
            requestQmlDialog(this, "deleteSupportDlg");
        }

        return true;
    }

    void TranslateMode::accept()
    {
        setMessage(true);
    }

    void TranslateMode::cancel()
    {
        setMessage(false);
        getKernelUI()->switchPickMode();
    }

    void TranslateMode::onLanguageChanged(MultiLanguage language)
    {
        m_name = tr("Move") + ": M";
    }

    void TranslateMode::onThemeChanged(ThemeCategory category)
    {
        setDisabledIcon(category == ThemeCategory::tc_dark ? "qrc:/UI/photo/leftBar/move_dark.svg" : "qrc:/UI/photo/leftBar/move_lite.svg");
        setEnabledIcon(category == ThemeCategory::tc_dark ? "qrc:/UI/photo/leftBar/move_dark.svg" : "qrc:/UI/photo/leftBar/move_lite.svg");
        setHoveredIcon(category == ThemeCategory::tc_dark ? "qrc:/UI/photo/leftBar/move_pressed.svg" : "qrc:/UI/photo/leftBar/move_lite.svg");
        setPressedIcon(category == ThemeCategory::tc_dark ? "qrc:/UI/photo/leftBar/move_pressed.svg" : "qrc:/UI/photo/leftBar/move_pressed.svg");
    }

    void TranslateMode::execute()
    {
        if (!m_translateOp)
        {
            m_translateOp = new TranslateOp(this);
            disconnect(m_translateOp, SIGNAL(positionChanged()), this, SIGNAL(positionChanged()));
            connect(m_translateOp, SIGNAL(positionChanged()), this, SIGNAL(positionChanged()));
        }

            setVisOperationMode(m_translateOp);


        emit positionChanged();
    }

    void TranslateMode::reset()
    {
        m_translateOp->reset();
    }

    QVector3D TranslateMode::position()
    {
        QVector3D pos;
        if (!m_translateOp)
            pos = QVector3D(0, 0, 0);
        else 
            pos = m_translateOp->position();
        return pos;
    }

    float TranslateMode::positionX()
    {
        return position().x();
    }

    float TranslateMode::positionY()
    {
        return position().y();
    }

    float TranslateMode::positionZ()
    {
        return position().z();
    }

    void TranslateMode::setQmlPosition(float val, int nXYZFlag)
    {
        if (nXYZFlag > 2)
            return;

        QVector3D oldPos = m_translateOp->position();
        switch (nXYZFlag)
        {
        case 0:
            // m_op->setScale(QVector3D(scaleValue, oldScale.y(), oldScale.z()));
            m_translateOp->setPosition(QVector3D(val, oldPos.y(), oldPos.z()));
            //emit xPositionChanged();
            break;
        case 1:
            m_translateOp->setPosition(QVector3D(oldPos.x(), val, oldPos.z()));
            //emit yPositionChanged();
            break;
        case 2:
            m_translateOp->setPosition(QVector3D(oldPos.x(), oldPos.y(), val));
            //emit zPositionChanged();
            break;
        default:
            break;
        }
    }

    void TranslateMode::setPosition(QVector3D position)
    {
        m_translateOp->setPosition(position);
    }

    void TranslateMode::center()
    {
        m_translateOp->center();
    }

    void TranslateMode::bottom()
    {
        m_translateOp->setBottom();
    }

    bool TranslateMode::selected()
    {
        QList<ModelN*> selections = selectionms();
        if (selections.size() > 0)
        {
            return true;
        }
        return false;
    }

    void TranslateMode::blockSignalMoveChanged(bool block)
    {
        if (m_translateOp)
        {
            m_translateOp->blockSignals(block);
        }
    }
}
