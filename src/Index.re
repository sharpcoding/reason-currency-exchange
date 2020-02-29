open ListItemsModels;

ExchangeData.fetchData("CHF");

ReactDOMRe.renderToElementWithId(<ListItemsComponent items={Some(sampleCurrencies)} />, "exchangeRatesList");