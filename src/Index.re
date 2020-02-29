open ListItemsModels;

Js.log(sampleCurrencies);

ReactDOMRe.renderToElementWithId(<ListItemsComponent items={Some(sampleCurrencies)} />, "exchangeRatesList");