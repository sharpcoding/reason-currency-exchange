open Belt;
open ListItemsModels;

let renderList = (items: option(listItems)) => {
  switch (items) {
  | Some(items) =>
    let theList = List.map((items), 
      (item) => <ListItemComponent item={item} />
    );
    <div> {React.array(List.toArray(theList))} </div>;
  | None => <div />
  };
};

[@react.component]
let make = (~items: option(listItems)) => {
  renderList(items);
};
