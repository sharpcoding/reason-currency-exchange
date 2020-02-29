type listItemAction =
  | Remove
  | MoveUp
  | MoveDown;

let actionToString = (~action: listItemAction) => {
    switch (action) {
    | Remove => "remove"
    | MoveUp => "move up"
    | MoveDown => "move down"
    };
  };

type listItem = {
  id: string,
  caption: string,
  actions: list(listItemAction),
};