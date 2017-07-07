#include "studiosequence.h"
#include "device.h"


using namespace  Framework;


Framework::StudioSequence::StudioSequence(uint32_t nr, QString name)
    : QObject()
    , m_isOpen(true)
    , m_seqNr(nr)
    , m_lastError(eErrors::noError)
    , m_name(name)
    , m_seqDevices(){
}

Framework::StudioSequence::~StudioSequence(){
}

void Framework::StudioSequence::append(std::weak_ptr<Device> d){
    if(m_isOpen){
        m_seqDevices.append(std::make_shared<markedDevice>(d));
        if(auto dev = d.lock()){
            connect(dev.get(), SIGNAL(cmdSequenceDone(std::weak_ptr<Device>, Errors::Id)), this, SLOT(onDevSequenceDone(std::weak_ptr<Device>, Errors::Id)));
        }
    }
}

void Framework::StudioSequence::storeFunction(std::function<int ()> f){
    m_fct = f;
    if(!m_isOpen && m_seqDevices.isEmpty()){
        m_fct();
    }
}

void Framework::StudioSequence::fireReadyDevices(){
    QMutableListIterator<std::shared_ptr<markedDevice> >iter(m_seqDevices);
    while(iter.hasNext()){
        auto md = iter.next();
        if(md->ready){
            iter.remove();
        }
    }
    if(m_seqDevices.isEmpty()){
        if(m_fct){
            if(m_lastError == eErrors::noError){
                m_fct();
            }
        }
        emit devSequenceDone(shared_from_this());
    }
}

void Framework::StudioSequence::onDevSequenceDone(std::weak_ptr<Device> dev, Errors::Id error){
    if(error != Errors::Id::E_NO_ERROR){
        m_lastError = eErrors::eDeviceCmd;
    }

    auto mdev = std::make_shared<markedDevice>(dev);
    if(m_seqDevices.contains(mdev)){
        // be careful, check if the sequence is just open, if only one element is in, the function hasn't to be fired: Reason:
        // if stored function is e.g. a join network from a scoro and the sequence done function was a genLamp (fired unconditional with migrateToNetwork call)
        // than the stored function is called. In this case we call migrateToNetwork from the sap!!! --> all further devices are going to NIRVANA (that's ugly bad).
        // mark the sequence device as ready and fire if the sequence will be closed

        if(m_seqDevices.size() > 1){
            if(m_seqDevices.removeOne(mdev)){
                if(auto d = dev.lock()){
                    disconnect(d.get(),SIGNAL(cmdSequenceDone(std::weak_ptr<Device>, Errors::Id)), this, SLOT(onDevSequenceDone(std::weak_ptr<Device>, Errors::Id)));
                }
            }
        }
        else{
            // if cloesed, normal handling
            if(!m_isOpen){
                if(m_seqDevices.removeOne(mdev)){
                    if(auto d = dev.lock()){
                        disconnect(d.get(),SIGNAL(cmdSequenceDone(std::weak_ptr<Device>, Errors::Id)), this, SLOT(onDevSequenceDone(std::weak_ptr<Device>, Errors::Id)));
                    }
                    if(m_seqDevices.size() < 1){
                        if(m_fct){
                            if(m_lastError == eErrors::noError){
                                m_fct();
                            }
                        }
                        emit devSequenceDone(shared_from_this());
                    }
                }
            }
            else{
                QListIterator<std::shared_ptr<markedDevice> >iter(m_seqDevices);
                while(iter.hasNext()){
                    auto md = iter.next();
                    auto mddevice = md->sDevice.lock();
                    auto d = dev.lock();
                    if(d && mddevice){
                        if(mddevice->getHiddenId() == d->getHiddenId()){
                            md->ready = true;
                            disconnect(d.get(),SIGNAL(cmdSequenceDone(std::weak_ptr<Device>, Errors::Id)), this, SLOT(onDevSequenceDone(std::weak_ptr<Device>, Errors::Id)));
                        }
                    }
                }
            }
        }
    }
}


bool Framework::operator==(const markedDevice &rhs, const markedDevice &lhs){
    auto slhs = lhs.sDevice.lock();
    auto rlhs = rhs.sDevice.lock();
    if(slhs && rlhs){
        return slhs->getHiddenId() == rlhs->getHiddenId();
    }
    else return false;
}
