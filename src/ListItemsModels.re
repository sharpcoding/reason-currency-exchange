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

type listItems = list(listItem);

let sampleCurrencies: listItems = [
  {id: "CHF", caption: "CHF/PLN 3.35", actions: [Remove]},
  {
    id: "USD",
    caption: "USD/PLN 3.35",
    actions: [Remove, MoveUp, MoveDown],
  },
  {
    id: "EUR",
    caption: "EUR/PLN 4.21",
    actions: [Remove, MoveUp, MoveDown],
  },
];