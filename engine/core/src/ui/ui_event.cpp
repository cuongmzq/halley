#include "ui/ui_event.h"
#include "halley/support/exception.h"

using namespace Halley;

UIEvent::UIEvent()
	: type(UIEventType::Undefined)
{
}

UIEvent::UIEvent(UIEventType type, String sourceId, String data)
	: type(type)
	, sourceId(sourceId)
	, data(data)
{
}

UIEvent::UIEvent(UIEventType type, String sourceId, int data)
	: type(type)
	, sourceId(sourceId)
	, intData(data)
{
}

UIEvent::UIEvent(UIEventType type, String sourceId, float data)
	: type(type)
	, sourceId(sourceId)
	, floatData(data)
{
}

UIEvent::UIEvent(UIEventType type, String sourceId, String data, int intData)
	: type(type)
	, sourceId(sourceId)
	, data(data)
	, intData(intData)
{
}

UIEvent::UIEvent(UIEventType type, String sourceId, Rect4f data)
	: type(type)
	, sourceId(sourceId)
	, rectData(data)
{
}

int UIEvent::getIntData() const
{
	return intData;
}

float UIEvent::getFloatData() const
{
	return floatData;
}

Rect4f UIEvent::getRectData() const
{
	return rectData;
}

UIEventType UIEvent::getType() const
{
	return type;
}

String UIEvent::getSourceId() const
{
	return sourceId;
}

String UIEvent::getData() const
{
	return data;
}

void UIEventHandler::setHandle(UIEventType type, UIEventCallback handler)
{
	handles[type] = handler;
}

void UIEventHandler::setHandle(UIEventType type, String id, UIEventCallback handler)
{
	specificHandles[std::make_pair(type, id)] = handler;
}

bool UIEventHandler::canHandle(const UIEvent& event) const
{
	if (specificHandles.find(std::make_pair(event.getType(), event.getSourceId())) != specificHandles.end()) {
		return true;
	} else {
		return handles.find(event.getType()) != handles.end();
	}
}

void UIEventHandler::queue(const UIEvent& event)
{
	eventQueue.push_back(event);
}

void UIEventHandler::pump()
{
	while (!eventQueue.empty()) {
		decltype(eventQueue) events = std::move(eventQueue);
		eventQueue.clear();
		for (auto& event: events) {
			handle(event);
		}
	}
}

void UIEventHandler::handle(const UIEvent& event) const
{
	auto iter = specificHandles.find(std::make_pair(event.getType(), event.getSourceId()));
	if (iter != specificHandles.end()) {
		iter->second(event);
	} else {
		auto iter2 = handles.find(event.getType());
		if (iter2 != handles.end()) {
			iter2->second(event);
		} else {
			throw Exception("Unable to handle event!");
		}
	}
}
