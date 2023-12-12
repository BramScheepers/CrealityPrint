#ifndef HOLLOWCOMMAND_H_
#define HOLLOWCOMMAND_H_

#include <QPointer>

#include "qtusercore/plugin/toolcommand.h"
#include "qtuser3d/module/pickableselecttracer.h"
#include "qtuser3d/module/pickable.h"
#include "hollowoperatemode.h"

class HollowCommand : public ToolCommand
                    , public qtuser_3d::SelectorTracer {
  Q_OBJECT;

public:
  explicit HollowCommand(QObject* parent = nullptr);
  virtual ~HollowCommand();

public:
  Q_INVOKABLE virtual bool checkSelect() override;

public:
  Q_PROPERTY(float minLengthInSelectionms
             READ getMinLengthInSelectionms
             NOTIFY minLengthInSelectionmsChanged);
  Q_INVOKABLE float getMinLengthInSelectionms();
  Q_SIGNAL void minLengthInSelectionmsChanged();

public:
  Q_INVOKABLE void execute();
  Q_INVOKABLE void hollow(float thinkness, float fill_ratio, bool enable_fill);

  Q_INVOKABLE float getThickness() const;
  //Q_INVOKABLE void setThickness(float thinkness);

  Q_INVOKABLE bool isFillEnabled() const;
  Q_INVOKABLE void setFillEnabled(bool enabled);

  Q_INVOKABLE float getFillRatio() const;
  Q_INVOKABLE void setFillRatio(float ratio);

protected:
  virtual void onSelectionsChanged() override;
  virtual void selectChanged(qtuser_3d::Pickable* pickable) override;

private:
  float thinkness_;
  QPointer<HollowOperateMode> operation_mode_;
};

#endif // HOLLOWCOMMAND_H_
