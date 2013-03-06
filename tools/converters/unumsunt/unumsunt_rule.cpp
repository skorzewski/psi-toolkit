#include "unumsunt_rule.hpp"


#include <sstream>

#include <boost/foreach.hpp>

#include "zvalue.hpp"


UnumsuntRule::operator std::string() const {
    std::stringstream sstr;
    {
        std::string comma(" for ");
        BOOST_FOREACH(std::string word, words) {
            sstr << comma << "[" << word << "]";
            comma = " or ";
        }
    }
    {
        std::string comma(" if ");
        BOOST_FOREACH(StringPair condition, conditions) {
            sstr << comma << "[" << condition.first << "] == [" << condition.second << "]";
            comma = " and ";
        }
    }
    {
        std::string comma(" then ");
        BOOST_FOREACH(StringPair command, commands) {
            sstr << comma << "[" << command.first << "] := [" << command.second << "]";
            comma = ", ";
        }
    }
    return sstr.str();
}


void UnumsuntRule::addCondition(std::string arg, std::string val) {
    conditions.push_back(StringPair(arg, val));
}


void UnumsuntRule::addCommand(std::string arg, std::string val) {
    commands.push_back(StringPair(arg, val));
}


void UnumsuntRule::addWord(std::string word) {
    words.insert(word);
}


bool UnumsuntRule::apply(AnnotationItemManager & manager, AnnotationItem & item) {
    if (!words.empty() && !words.count(item.getText())) {
        return false;
    }
    BOOST_FOREACH(StringPair condition, conditions) {
        if (condition.first == "CAT") {
            if (condition.second != item.getCategory()) {
                return false;
            }
        } else {
            if (manager.getValueAsString(item, condition.first) != condition.second) {
                return false;
            }
        }
    }
    BOOST_FOREACH(StringPair command, commands) {
        if (command.first == "CAT") {
            if (command.second[0] == '$') {
                manager.setCategory(item,
                    manager.getValueAsString(item, command.second.substr(1)));
            } else {
                manager.setCategory(item, command.second);
            }
        } else {
            if (command.second[0] == '$') {
                manager.setValue(item, command.first,
                    manager.getValue(item, command.second.substr(1)));
            } else if (command.second == "0") {
                manager.setValue(item, command.first, manager.any_value());
            } else if (command.second == "-1") {
                manager.setValue(item, command.first, manager.false_value());
            } else {
                manager.setValue(item, command.first, command.second);
            }
        }
    }
    return true;
}