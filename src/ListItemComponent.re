open Belt;
open ControlModels;

let actionsAsString = (~item: listItem) => List.reduce(
  item.actions, 
  "",
  (acc, el) => acc ++ "; " ++ actionToString(el));

[@react.component]
let make = (~item: listItem) => {
  
  <div key={item.id}>
    {ReasonReact.string(item.caption)}
    {ReasonReact.string(" ")}
    {ReasonReact.string(actionsAsString(item))}
  </div>;
}