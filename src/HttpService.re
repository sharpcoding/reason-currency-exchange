open Belt;
open AppModels;
open AppSettings;

let url = currencyCode => {j|$nbpApiBaseUrl/$currencyCode/|j};

module JsonOps = {
  type jsonExchangeRateItem = {
    no: string,
    effectiveDate: string,
    mid: float,
  };

  type jsonExchangeRateResponse = {
    table: string,
    currency: string,
    code: string,
    rates: array(jsonExchangeRateItem),
  };

  let exchangeRateItemDecoder = (json): jsonExchangeRateItem => {
    Json.Decode.{
      no: json |> field("no", string),
      effectiveDate: json |> field("effectiveDate", string),
      mid: json |> field("mid", float),
    };
  };

  let exchangeRateDecoder = (json): jsonExchangeRateResponse => {
    Json.Decode.{
      table: json |> field("table", string),
      currency: json |> field("currency", string),
      code: json |> field("code", string),
      rates: json |> field("rates", array(exchangeRateItemDecoder)),
    };
  };
};

module Dto = {
  let jsonToCurrencyExchangeModel =
      (json: JsonOps.jsonExchangeRateResponse): currencyExchangeRatePoint => {
    switch (List.fromArray(json.rates)) {
    | [] => {currencyCode: json.code, point: None}
    | [head, ..._] => {
        currencyCode: json.code,
        point: Some({rate: head.mid, date: head.effectiveDate}),
      }
    };
  };
};

module InternalHttpOps = {
  let fetchCurrency = currencyCode => {
    Js.Promise.(
      Fetch.fetch(url(currencyCode))
      |> then_(Fetch.Response.json)
      |> then_(json =>
           json
           |> JsonOps.exchangeRateDecoder
           |> (
             transformed => {
               resolve(Dto.jsonToCurrencyExchangeModel(transformed));
             }
           )
         )
    );
  };
};

let fetchCurrencies = (currencyCodes: list(string), callback) => {
  Js.Promise.(
    Js.Promise.all(
      List.toArray(
        List.map(currencyCodes, code => InternalHttpOps.fetchCurrency(code)),
      ),
    )
    |> then_(response => {
         callback(List.fromArray(response));
         resolve();
       })
    |> catch((_) => {
      Js.log("error");
      resolve(callback([]));
    })
    |> ignore
  );
};
