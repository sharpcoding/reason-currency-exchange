open Belt;
open IsoCurrencies;

[@react.component]
let make = (~currencies: list(isoCurrencyDescription), ~onSelectCurrency) => {
  <select
    onChange={e => {
      let currencyCode = ReactEvent.Form.target(e)##value;
      onSelectCurrency(currencyCode);
    }}>
    {React.array(
       List.toArray(
         List.map(currencies, (c: isoCurrencyDescription) =>
           <option key={c.currencyCode} value={c.currencyCode}>
             {React.string(c.currencyCode ++ "-" ++ c.currencyDescription)}
           </option>
         ),
       ),
     )}
  </select>;
};
