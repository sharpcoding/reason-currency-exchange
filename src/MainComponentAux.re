open AppSettings;
open IsoCurrencies;

let popularityValue = (cd: isoCurrencyDescription) => {
  switch (cd.popularity) {
  | Top => 0
  | Popular => 1
  | LocallyPopular => 2
  | Exotic => 3
  };
};

let currenciesfilteredSorted = (~selectedCurrencies: list(string)) => {
  let isoCurrenciesSorted =
    List.sort(
      (a, b) => {
        let pva = popularityValue(a);
        let pvb = popularityValue(b);
        pva - pvb;
      },
      isoCurrencies,
    );

  List.filter(
    (a: isoCurrencyDescription) => {
      let found =
        try (List.find(e => e == a.currencyCode, selectedCurrencies)) {
        | Not_found => ""
        };
      found == "";
    },
    isoCurrenciesSorted,
  );
};

  let convertInAppCurrencyToListItem =
      (c: AppModels.currencyExchangeRatePoint): ControlModels.listItem => {
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

    let result: ControlModels.listItem = {
      id: c.currencyCode,
      caption,
      actions: [],
    };

    result;
  };
