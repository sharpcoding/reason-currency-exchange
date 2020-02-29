type ratePoint = {
  rate: float,
  date: string,
}

type currencyExchangeRatePoint = {
  currencyCode: string,
  point: option(ratePoint),
};