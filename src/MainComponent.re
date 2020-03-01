open Belt;
open AppModels;
open AppSettings;
open ControlModels;
open MainComponentAux;

type httpOperation =
  | None
  | AllCurrenciesLoading
  | SingleCurrencyLoading;

type state = {
  inSelectCurrencies: list(IsoCurrencies.isoCurrencyDescription),
  currencyInfo: list(currencyExchangeRatePoint),
  userSelectedCurrencyCode: string,
  httpOperation,
};

type action =
  | StartLoadingAllSelectedCurrencies
  | FinishLoadingAllSelectedCurrencies(list(currencyExchangeRatePoint))
  | SetUserSelectedCurrency(string)
  | RemoveCurrency(string)
  | StartLoadingSingleCurrency(string)
  | FinishLoadingSingleCurrency(currencyExchangeRatePoint);

let initialCurrencyInfo = [
  {currencyCode: "USD", point: None},
  {currencyCode: "EUR", point: None},
  {currencyCode: "CHF", point: None},
];

let initialCurrenciesFilteredSorted =
  currenciesfilteredSorted(
    ~selectedCurrencies=List.map(initialCurrencyInfo, c => c.currencyCode),
  );

let initialState = {
  inSelectCurrencies: initialCurrenciesFilteredSorted,
  userSelectedCurrencyCode:
    switch (initialCurrenciesFilteredSorted) {
    | [] => ""
    | [h, ..._] => h.currencyCode
    },
  currencyInfo: initialCurrencyInfo,
  httpOperation: None,
};

[@react.component]
let make = () => {
  let (state, dispatch) =
    React.useReducer(
      (state, action) =>
        switch (action) {
        | StartLoadingAllSelectedCurrencies => {
            ...state,
            httpOperation: AllCurrenciesLoading,
          }
        | FinishLoadingAllSelectedCurrencies(payload) => {
            ...state,
            currencyInfo: payload,
            httpOperation: None,
          }
        | SetUserSelectedCurrency(currencyCode) => {
            ...state,
            userSelectedCurrencyCode: currencyCode,
          }
        | RemoveCurrency(currencyCode) =>
          let newCurrencyInfo =
            List.reduce(state.currencyInfo, [], (acc, el) =>
              switch (el.currencyCode == currencyCode) {
              | false => List.concat(acc, [el])
              | true => acc
              }
            );
          let newInSelectCurrencies =
            currenciesfilteredSorted(
              ~selectedCurrencies=
                List.map(newCurrencyInfo, c => c.currencyCode),
            );
          {
            ...state,
            currencyInfo: newCurrencyInfo,
            inSelectCurrencies: newInSelectCurrencies,
          };
        | StartLoadingSingleCurrency(currencyCode) =>
          switch (
            List.filter(state.currencyInfo, el =>
              el.currencyCode == currencyCode
            )
          ) {
          | [a] => {...state, httpOperation: SingleCurrencyLoading}
          | _ => {
              ...state,
              currencyInfo:
                List.concat(
                  state.currencyInfo,
                  [{currencyCode, point: None}],
                ),
              httpOperation: SingleCurrencyLoading,
            }
          }
        | FinishLoadingSingleCurrency(payload) =>
          let newCurrencyInfo =
            List.reduce(
              state.currencyInfo,
              [],
              (acc, el) => {
                let ne =
                  switch (el.currencyCode == payload.currencyCode) {
                  | false => el
                  | true => {
                      currencyCode: el.currencyCode,
                      point: payload.point,
                    }
                  };
                List.concat(acc, [ne]);
              },
            );
          let newInSelectCurrencies =
            currenciesfilteredSorted(
              ~selectedCurrencies=
                List.map(newCurrencyInfo, c => c.currencyCode),
            );
          {
            userSelectedCurrencyCode:
              switch (newInSelectCurrencies) {
              | [] => ""
              | [h, ..._] => h.currencyCode
              },
            currencyInfo: newCurrencyInfo,
            inSelectCurrencies: newInSelectCurrencies,
            httpOperation: None,
          };
        },
      initialState,
    );

  React.useEffect0(() => {
    dispatch(StartLoadingAllSelectedCurrencies);
    None;
  });

  React.useEffect1(
    () =>
      switch (state.httpOperation) {
      | SingleCurrencyLoading =>
        HttpService.fetchCurrencies(
          [state.userSelectedCurrencyCode],
          (response: list(currencyExchangeRatePoint)) =>
          switch (response) {
          | [] => ()
          | [h, ..._] => dispatch(FinishLoadingSingleCurrency(h))
          }
        )
        |> ignore;
        None;
      | AllCurrenciesLoading =>
        let currencies = List.map(state.currencyInfo, ci => ci.currencyCode);
        HttpService.fetchCurrencies(
          currencies, (response: list(currencyExchangeRatePoint)) =>
          dispatch(FinishLoadingAllSelectedCurrencies(response))
        )
        |> ignore;
        None;
      | _ => None
    },
    List.toArray([state.httpOperation]),
  );

  <>
    <h3> {React.string("ReasonML demo exchange rate application")} </h3>
    <div>
      <CurrenciesComponent
        currencies={state.inSelectCurrencies}
        onSelectCurrency={currencyCode =>
          dispatch(SetUserSelectedCurrency(currencyCode))
        }
      />
      <button
        onClick={_ =>
          dispatch(
            StartLoadingSingleCurrency(state.userSelectedCurrencyCode),
          )
        }>
        {ReasonReact.string("Load currency to PLN exchange rate")}
      </button>
      {React.array(
         List.toArray(
           List.map(state.currencyInfo, (i: currencyExchangeRatePoint) =>
             <ListItemComponent
               key={i.currencyCode}
               item={convertInAppCurrencyToListItem(i)}
               onRemove={currencyCode =>
                 dispatch(RemoveCurrency(currencyCode))
               }
             />
           ),
         ),
       )}
      <button onClick={_ => dispatch(StartLoadingAllSelectedCurrencies)}>
        {ReasonReact.string("Refresh all rates")}
      </button>
      <hr />
      {state.httpOperation == None
         ? <div> {React.string("Incative")} </div>
         : <div> {React.string("Loading...")} </div>}
    </div>
  </>;
};
