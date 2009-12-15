/**
 *	Implementation file for the pmvcpp architecture.
 *
 *	@author	Schell Scivally
 */
#include <iostream>
#include "patterns.h"
#include "pmvcpparch.h"
#include "pmvcppexp.h"
//--------------------------------------
//  MultitonKeyHeir
//--------------------------------------
void MultitonKeyHeir::setMultitonKey(std::string key)
{
    this->_multitonKey = key;
}

std::string MultitonKeyHeir::getMultitonKey()
{
    return this->_multitonKey;
}
//--------------------------------------
//  Notification
//--------------------------------------
Notification::Notification(std::string name, IBody* body, std::string type)
{
    this->name = name;
    this->setBody(body);
    this->setType(type);
}
Notification::Notification(std::string name, IBody* body)
{
    this->name = name;
    this->setBody(body);
}
Notification::Notification(std::string name, std::string type)
{
    this->name = name;
    this->setType(type);
}
std::string Notification::getName()
{
    return this->name;
}
void Notification::setBody( IBody* body )
{
    this->body = body;
}
IBody* Notification::getBody()
{
    return this->body;
}
void Notification::setType( std::string type )
{
    this->type = type;
}
std::string Notification::getType()
{
    return this->type;
}
//--------------------------------------
//  Notifier
//--------------------------------------
void Notifier::sendNotification( std::string notificationName, IBody* body, std::string type)
{

}
void Notifier::sendNotification( std::string notificationName, std::string type )
{

}
void Notifier::sendNotification( std::string notificationName, IBody* body )
{

}
void Notifier::sendNotification( std::string notificationName )
{

}
void Notifier::initializeNotifier(std::string key)
{
    this->setMultitonKey(key);
}
IFacade* Notifier::getFacade()
{
    return Multiton<Facade>::instance(this->getMultitonKey());
}
//--------------------------------------
//  SimpleCommand
//--------------------------------------
void SimpleCommand::execute(INotification* notification)
{
    
}
//--------------------------------------
//  MacroCommand
//--------------------------------------
MacroCommand::MacroCommand()
{
}
void MacroCommand::addSubCommand(ICommand* command)
{
    this->subCommands.push_back(command);
}
void MacroCommand::execute(INotification* notification)
{
    std::vector<ICommand*>::iterator it;
    for(it = this->subCommands.begin(); it != this->subCommands.end(); it++)
    {
        (*it)->execute(notification);
    }
}
//--------------------------------------
//  Observer
//--------------------------------------
// defined in header
//--------------------------------------
//  Proxy
//--------------------------------------
// defined in header
//--------------------------------------
//  Model
//--------------------------------------
Model::Model()
{}
void Model::registerProxy( IRegisterable* proxy )
{}
IRegisterable* Model::retrieveProxy( std::string proxyName )
{return 0;}
bool Model::hasProxy( std::string proxyName )
{return true;}
IRegisterable* Model::removeProxy( std::string proxyName )
{return 0;}
void Model::removeModel( std::string key )
{}
void Model::initializeModel(  )
{}
//--------------------------------------
//  View
//--------------------------------------
View::View( )
{
}
IView* View::getInstance(std::string key)
{
    // if the instance already exists, simply return it
    if(Multiton<View>::exists(key))
        return Multiton<View>::instance(key);
    // if the instance needs to be created, do so and take
    // care of some initialization
    View* viewPtr = Multiton<View>::instance(key);
    viewPtr->setMultitonKey(key);
    viewPtr->initializeView();
    return viewPtr;
}
void View::registerObserver ( std::string notificationName, IObserverFunctor* observer )
{
    if(! this->existsObserversInterestedIn(notificationName))
    {
        std::vector<IObserverFunctor*> observerVector;
        observerVector.push_back(observer);
        this->observerMap[notificationName] = observerVector;
    }
    else
    {
        this->observerMap[notificationName].push_back(observer);
    }
}
void View::notifyObservers( INotification* notification )
{
    std::string noteName = notification->getName();
    if(this->existsObserversInterestedIn(noteName))
    {
        // there was some stuff about the reference array changing while
        // running this loop, although i don't see how that could happen
        // until we add threads to this architecture - so i'm keeping
        // it simple here
        std::vector<IObserverFunctor*> observers = this->observerMap[noteName];
        std::vector<IObserverFunctor*>::iterator it;
        for(it = observers.begin(); it != observers.end(); it++)
        {
            (*it)->notifyObserver(notification);
        }
    }
}
void View::removeObserver( std::string notificationName, unsigned int contextAddress )
{
    if(this->existsObserversInterestedIn(notificationName))
    {
        std::vector<IObserverFunctor*> observers = this->observerMap[notificationName];
        std::vector<IObserverFunctor*>::iterator it;

        for(it = observers.begin(); it != observers.end(); it++)
        {
            // this is tricky because in c++ we can't compare arbitrary types
            // we have to downcast the IObserverFunctors to their derived classes
            // in order to properly compare them. someone may object to this -
            // i know it's programming taboo - so by all means figure out a
            // better way to do this, however i'm sure  it will
            // involve refactoring the base classes
            if((*it)->compareNotifyContext(contextAddress) == true)
            {
                observers.erase(it);
                break;
            }
        }

        if(observers.size() == (size_t) 0)
           this->observerMap.erase(notificationName);
    }
}
void View::registerMediator( IRegisterable* mediator )
{

}
IRegisterable* View::retrieveMediator( std::string mediatorName )
{return 0;}
IRegisterable* View::removeMediator( std::string mediatorName )
{return 0;}
bool View::hasMediator( std::string mediatorName )
{return true;}
void View::removeView( std::string key )
{}
void View::initializeView()
{}
bool View::existsObserversInterestedIn(std::string notificationName)
{
    return this->observerMap.find(notificationName) != this->observerMap.end();
}
//--------------------------------------
//  Controller
//--------------------------------------
Controller::Controller()
{
}
IController* Controller::getInstance(std::string key)
{
    // if the instance already exists, simply return it
    if(Multiton<Controller>::exists(key))
        return Multiton<Controller>::instance(key);
    // if the instance needs to be created, do so and take
    // care of some initialization
    Controller* contPtr = Multiton<Controller>::instance(key);
    contPtr->setMultitonKey(key);
    contPtr->initializeController();
    return contPtr;
}
void Controller::executeCommand( INotification* note )
{}
void Controller::registerCommand( std::string notificationName, ICommand* commandClassRef )
{}
bool Controller::hasCommand( std::string notificationName )
{}
void Controller::removeCommand( std::string notificationName )
{}
void Controller::removeController( std::string key )
{}
void Controller::initializeController( )
{
    this->view = View::getInstance(this->getMultitonKey());
}
//--------------------------------------
//  FACADE
//--------------------------------------
Facade::Facade()
{
    // make sure our pointers are null at first
    // for testing purposes
    this->model = 0;
    this->controller = 0;
    this->view = 0;
}
void Facade::registerCommand( std::string notificationName, ICommand* commandClassRef )
{}
void Facade::removeCommand( std::string notificationName )
{}
bool Facade::hasCommand( std::string notificationName )
{return true;}
void Facade::registerProxy ( IRegisterable* proxy )
{}
IRegisterable* Facade::retrieveProxy ( std::string proxyName )
{return 0;}
IRegisterable* Facade::removeProxy ( std::string proxyName )
{return 0;}
bool Facade::hasProxy( std::string proxyName )
{return true;}
void Facade::registerMediator( IRegisterable* mediator )
{}
IRegisterable* Facade::retrieveMediator( std::string mediatorName )
{return 0;}
IRegisterable* Facade::removeMediator( std::string mediatorName )
{return 0;}
bool Facade::hasMediator( std::string mediatorName )
{return true;}
void Facade::sendNotification( std::string notificationName, IBody* body, std::string type)
{}
void Facade::sendNotification( std::string notificationName, std::string type )
{}
void Facade::sendNotification( std::string notificationName, IBody* body )
{}
void Facade::sendNotification( std::string notificationName )
{}
void Facade::notifyObservers ( INotification* notification )
{}
void Facade::initializeNotifier(std::string key)
{
    this->setMultitonKey(key);
}
bool Facade::hasCore( std::string key )
{return true;}
void Facade::removeCore( std::string key )
{}
void Facade::initializeFacade(std::string key)
{
    this->initializeNotifier(key);
    this->initializeModel();
    this->initializeController();
    this->initializeView();
}

void Facade::initializeModel()
{
    // if it's been initialized, abort
    if(this->controller != (IController*) 0)
        return;
    this->controller = Multiton<Controller>::instance(this->getMultitonKey());
    this->controller->setMultitonKey(this->getMultitonKey());
}
void Facade::initializeController()
{
    // if it's been initialized, abort
    if(this->model != (IModel*) 0)
        return;
    this->model = Multiton<Model>::instance(this->getMultitonKey());
    this->model->setMultitonKey(this->getMultitonKey());
}
void Facade::initializeView()
{
    // if it's been initialized, abort
    if(this->view != (IView*) 0)
        return;
    this->view = Multiton<View>::instance(this->getMultitonKey());
    this->view->setMultitonKey(this->getMultitonKey());
}