open CurrencyExchangeModels;

let apiBaseUrl = "http://api.nbp.pl/api/exchangerates/rates/a";
let url = currencyCode => {j|$apiBaseUrl/$currencyCode/|j};

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

module JsonDecode = {
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
      (~json: jsonExchangeRateResponse): option(currencyExchangeSample) => {
    switch (Belt.List.fromArray(json.rates)) {
    | [] => None
    | [head, ..._] =>
      Some({
        currencyCode: json.code,
        exchangeRate: head.mid,
        date: head.effectiveDate,
      })
    };
  };
  let jsonToCurrencyExchangeModel2 =
      (~json: jsonExchangeRateResponse): currencyExchangeSample => {
    let result: currencyExchangeSample = {
      currencyCode: json.code,
      exchangeRate: 2.33,
      date: "222",
    };
    result;
  };
};

let fetchData = currencyCode => {
  Js.Promise.(
    Fetch.fetch(url(currencyCode))
    |> then_(Fetch.Response.json)
    |> then_(json =>
         json
         |> JsonDecode.exchangeRateDecoder
         |> (
           transformed => {
             Js.log(Dto.jsonToCurrencyExchangeModel(transformed));
             resolve();
           }
         )
       )
    |> ignore
  );
};
