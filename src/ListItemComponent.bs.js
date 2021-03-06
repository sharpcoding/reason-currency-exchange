// Generated by BUCKLESCRIPT, PLEASE EDIT WITH CARE
'use strict';

var Curry = require("bs-platform/lib/js/curry.js");
var React = require("react");
var Belt_List = require("bs-platform/lib/js/belt_List.js");
var ControlModels$ReactHooksTemplate = require("./ControlModels.bs.js");

function actionsAsString(item) {
  return Belt_List.reduce(item[/* actions */2], "", (function (acc, el) {
                return acc + ("; " + ControlModels$ReactHooksTemplate.actionToString(el));
              }));
}

function ListItemComponent(Props) {
  var item = Props.item;
  var onRemove = Props.onRemove;
  return React.createElement("div", undefined, item[/* caption */1], " ", actionsAsString(item), " ", React.createElement("button", {
                  onClick: (function (param) {
                      return Curry._1(onRemove, item[/* id */0]);
                    })
                }, "Remove"));
}

var make = ListItemComponent;

exports.actionsAsString = actionsAsString;
exports.make = make;
/* react Not a pure module */
