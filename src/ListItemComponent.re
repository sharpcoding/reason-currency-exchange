open Belt;
open ControlModels;

let actionsAsString = (item: listItem) =>
  List.reduce(item.actions, "", (acc, el) =>
    acc ++ "; " ++ actionToString(el)
  );

[@react.component]
let make = (~item: listItem, ~onRemove) => {
  <div>
    {ReasonReact.string(item.caption)}
    {ReasonReact.string(" ")}
    {ReasonReact.string(actionsAsString(item))}
    {ReasonReact.string(" ")}
    {<button onClick={_ => onRemove(item.id)}>
       {ReasonReact.string("Remove")}
     </button>}
  </div>;
};
