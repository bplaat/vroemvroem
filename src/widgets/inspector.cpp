// VroemVroem - Inspector Widget

#include "widgets/inspector.hpp"
#include "widgets/label.hpp"
#include "objects/nature.hpp"
#include "objects/house.hpp"
#include "objects/road.hpp"
#include "objects/city.hpp"
#include "objects/vehicle.hpp"
#include "fonts.hpp"

namespace Widgets {

Inspector::Inspector(const Objects::Object *object, std::unique_ptr<Rect> rect, std::unique_ptr<Color> backgroundColor, std::function<void()> clickCallback)
    : WidgetGroup::WidgetGroup(std::move(rect), std::move(backgroundColor), clickCallback), object(object)
{
    createWidgets();
}

const Objects::Object *Inspector::getObject() const {
    return object;
}

void Inspector::setObject(const Objects::Object *object) {
    visible = object != nullptr;
    this->object = object;

    widgets.clear();
    createWidgets();
}

void Inspector::draw(std::shared_ptr<Canvas> canvas) const {
    if (visible) {
        WidgetGroup::draw(canvas);
    }
}

void Inspector::createWidgets() {
    if (object) {
        // Get information via all subtypes
        const char *name;

        if (const Objects::Nature *nature = dynamic_cast<const Objects::Nature *>(object)) {
            name = Objects::Nature::getName(nature->getType());
        }

        if (const Objects::House *house = dynamic_cast<const Objects::House *>(object)) {
            name = Objects::House::getName(house->getType());
        }

        if (const Objects::Road *road = dynamic_cast<const Objects::Road *>(object)) {
            char roadNameString[128];
            sprintf(roadNameString, "Road (%d %s)", road->getLanes(), road->getLanes() == 1 ? "lane" : "lanes");
            name = roadNameString;
        }

        if (const Objects::City *city = dynamic_cast<const Objects::City *>(object)) {
            char cityNameString[128];
            sprintf(cityNameString, "City: %s", city->getName());
            name = cityNameString;
        }

        if (const Objects::Vehicle *vehicle = dynamic_cast<const Objects::Vehicle *>(object)) {
            const Objects::Vehicle::Stats *vehicleStats = Objects::Vehicle::getStats(vehicle->getType());
            name = vehicleStats->name;
        }

        int y = 16;

        // Create name label
        widgets.push_back(std::make_unique<Label>(
            name,
            std::move(std::make_unique<Rect>(rect->x + 16, rect->y + y, rect->width, 32)),
            Fonts::getInstance()->getTextFont(),
            32,
            std::move(std::make_unique<Color>(255, 255, 255)),
            Label::Align::LEFT,
            nullptr,
            nullptr
        ));

        y += 32 + 24;

        // Create id label
        char idString[128];
        sprintf(idString, "id = %d", object->getId());
        widgets.push_back(std::make_unique<Label>(
            idString,
            std::move(std::make_unique<Rect>(rect->x + 16, rect->y + y, rect->width, 24)),
            Fonts::getInstance()->getTextFont(),
            24,
            std::move(std::make_unique<Color>(255, 255, 255)),
            Label::Align::LEFT,
            nullptr,
            nullptr
        ));

        y += 24 + 16;

        // Create x label
        char xString[128];
        sprintf(xString, "x = %.02f", object->getX());
        widgets.push_back(std::make_unique<Label>(
            xString,
            std::move(std::make_unique<Rect>(rect->x + 16, rect->y + y, rect->width, 24)),
            Fonts::getInstance()->getTextFont(),
            24,
            std::move(std::make_unique<Color>(255, 255, 255)),
            Label::Align::LEFT,
            nullptr,
            nullptr
        ));

        y += 24 + 16;

        // Create y label
        char yString[128];
        sprintf(yString, "y = %.02f", object->getY());
        widgets.push_back(std::make_unique<Label>(
            yString,
            std::move(std::make_unique<Rect>(rect->x + 16, rect->y + y, rect->width, 24)),
            Fonts::getInstance()->getTextFont(),
            24,
            std::move(std::make_unique<Color>(255, 255, 255)),
            Label::Align::LEFT,
            nullptr,
            nullptr
        ));

        y += 24 + 16;

        // Create house population label
        if (const Objects::House *house = dynamic_cast<const Objects::House *>(object)) {
            char populationString[128];
            sprintf(populationString, "population = %d", house->getPopulation());
            widgets.push_back(std::make_unique<Label>(
                populationString,
                std::move(std::make_unique<Rect>(rect->x + 16, rect->y + y, rect->width, 24)),
                Fonts::getInstance()->getTextFont(),
                24,
                std::move(std::make_unique<Color>(255, 255, 255)),
                Label::Align::LEFT,
                nullptr,
                nullptr
            ));

            y += 24 + 16;
        }

        // Create road end x and end y label
        if (const Objects::Road *road = dynamic_cast<const Objects::Road *>(object)) {
            char endXString[128];
            sprintf(endXString, "endX = %.02f", road->getEndX());
            widgets.push_back(std::make_unique<Label>(
                endXString,
                std::move(std::make_unique<Rect>(rect->x + 16, rect->y + y, rect->width, 24)),
                Fonts::getInstance()->getTextFont(),
                24,
                std::move(std::make_unique<Color>(255, 255, 255)),
                Label::Align::LEFT,
                nullptr,
                nullptr
            ));

            y += 24 + 16;

            char endYString[128];
            sprintf(endYString, "endY = %.02f", road->getEndY());
            widgets.push_back(std::make_unique<Label>(
                endYString,
                std::move(std::make_unique<Rect>(rect->x + 16, rect->y + y, rect->width, 24)),
                Fonts::getInstance()->getTextFont(),
                24,
                std::move(std::make_unique<Color>(255, 255, 255)),
                Label::Align::LEFT,
                nullptr,
                nullptr
            ));

            y += 24 + 16;
        }

        // Create city population label
        if (const Objects::City *city = dynamic_cast<const Objects::City *>(object)) {
            char populationString[128];
            sprintf(populationString, "population = %d", city->getPopulation());
            widgets.push_back(std::make_unique<Label>(
                populationString,
                std::move(std::make_unique<Rect>(rect->x + 16, rect->y + y, rect->width, 24)),
                Fonts::getInstance()->getTextFont(),
                24,
                std::move(std::make_unique<Color>(255, 255, 255)),
                Label::Align::LEFT,
                nullptr,
                nullptr
            ));

            y += 24 + 16;
        }
    }
}

}
