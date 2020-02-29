open Belt;
open AppModels;
open AppSettings;
open ControlModels;

type state = {
  currencyInfo: list(currencyExchangeRatePoint),
  loading: bool,
};

type action =
  | Loaded(list(currencyExchangeRatePoint))
  | Loading;

let initialState = {
  currencyInfo: [
    {currencyCode: "USD", point: None},
    {currencyCode: "EUR", point: None},
    {currencyCode: "CHF", point: None},
    {currencyCode: "CAD", point: None},
    {currencyCode: "CZK", point: None},
  ],
  loading: false,
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | Loaded(payload) => {
            currencyInfo: payload,
            loading: false,
          }
        | Loading => {...state, loading: true}
        },
      initialState,
    );

  React.useEffect0(() => {
    dispatch(Loading);
    let currencies = Belt.List.map(state.currencyInfo, ci => ci.currencyCode);
    HttpService.fetchCurrencies(
      currencies, (response: list(currencyExchangeRatePoint)) =>
      dispatch(Loaded(response))
    )
    |> ignore;
    None;
  });

  let convertInAppCurrencyToListItem =
      (c: currencyExchangeRatePoint): listItem => {
    let rate = () => {
      switch (c.point) {
      | Some(p) => string_of_float(p.rate)
      | None => "N/A"
      };
    };

    let caption =
      "Exchange rate of "
      ++ baseCurrency
      ++ " to "
      ++ c.currencyCode
      ++ ": "
      ++ rate();

    let result: listItem = {
      id: baseCurrency ++ "-to-" ++ c.currencyCode,
      caption,
      actions: [],
    };

    result;
  };

  <>
    <h3> {React.string("ReasonML demo exchange rate application")} </h3>
    <div>
      {React.array(
         List.toArray(
           List.map(state.currencyInfo, (i: currencyExchangeRatePoint) =>
             <ListItemComponent item={convertInAppCurrencyToListItem(i)} />
           ),
         ),
       )}
       <hr />
       {
        switch (state.loading) {
          | true => <div>{React.string("Loading...")}</div> 
          | false => <div>{React.string("Data loaded !")}</div> 
        }
      }
    </div>
  </>;
};
