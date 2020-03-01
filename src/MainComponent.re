open Belt;
open AppModels;
open AppSettings;
open ControlModels;
open IsoCurrencies;

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
        | Loaded(payload) => {currencyInfo: payload, loading: false}
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
      | Some(p) => Js.Float.toString(p.rate)
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
      <select
        onChange={e => {
          let x = ReactEvent.Form.target(e)##value;
          Js.log(x);
        }}>
        {React.array(
           List.toArray(
             List.map(isoCurrencies, (c: isoCurrencyDescription) =>
               <option value={c.currencyCode}>
                 {React.string(c.currencyCode ++ "-" ++ c.currencyDescription)}
               </option>
             ),
           ),
         )}
      </select>
      {React.array(
         List.toArray(
           List.map(state.currencyInfo, (i: currencyExchangeRatePoint) =>
             <ListItemComponent
               key={i.currencyCode}
               item={convertInAppCurrencyToListItem(i)}
             />
           ),
         ),
       )}
      <hr />
      {state.loading
         ? <div> {React.string("Loading...")} </div>
         : <div> {React.string("Data loaded !")} </div>}
    </div>
  </>;
};
