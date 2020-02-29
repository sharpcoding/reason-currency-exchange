open CurrencyExchangeModels;
open ListItemsModels;

HttpService.fetchData("CHF", (response: option(currencyExchangeRatePoint)) => {
  switch response {
  | Some(x) => Js.log2("succeeded with", x);
  | None => Js.log("failed !");
  };
});

ReactDOMRe.renderToElementWithId(<ListItemsComponent items={Some(sampleCurrencies)} />, "exchangeRatesList");