#ifndef DEMODEVICEPARA_H
#define DEMODEVICEPARA_H

#include <devicepara.h>

namespace Framework{


class DemoDevicePara : public DevicePara{
public:
    DemoDevicePara(Device* parent, int id, const QVariant value, Types typer, float scale = 1.0f, DevicePara::Settings s = DevicePara::noOptions, preReadFct fct = &everValid, depFunction dfct = &noDependency);
    virtual ~DemoDevicePara(){}
    // only overwrite the setremote function to bypass communication
    virtual bool setRemote(const QVariant value, QList<std::function<bool()> > postCmd = QList<std::function<bool()> >()) override;
    virtual bool setRemote() override;
    virtual void getRemote() override;
    virtual bool isDemo() const override;

};

inline bool DemoDevicePara::isDemo() const{
    return true;
}


}
#endif // DEMODEVICEPARA_H
