/**
 * Реализация API, не изменяйте ее
 * @param {string} url
 * @param {function} callback
 */

function getData(url, callback) {
    var RESPONSES = {
        '/countries': [
            {name: 'Cameroon', continent: 'Africa'},
            {name :'Fiji Islands', continent: 'Oceania'},
            {name: 'Guatemala', continent: 'North America'},
            {name: 'Japan', continent: 'Asia'},
            {name: 'Yugoslavia', continent: 'Europe'},
            {name: 'Tanzania', continent: 'Africa'}
        ],
        '/cities': [
            {name: 'Bamenda', country: 'Cameroon'},
            {name: 'Suva', country: 'Fiji Islands'},
            {name: 'Quetzaltenango', country: 'Guatemala'},
            {name: 'Osaka', country: 'Japan'},
            {name: 'Subotica', country: 'Yugoslavia'},
            {name: 'Zanzibar', country: 'Tanzania'},
        ],
        '/populations': [
            {count: 138000, name: 'Bamenda'},
            {count: 77366, name: 'Suva'},
            {count: 90801, name: 'Quetzaltenango'},
            {count: 2595674, name: 'Osaka'},
            {count: 100386, name: 'Subotica'},
            {count: 157634, name: 'Zanzibar'}
        ]
    };

    setTimeout(function () {
        var result = RESPONSES[url];
        if (!result) {
            return callback('Unknown url');
        }

        callback(null, result);
    }, Math.round(Math.random * 1000));
}

/**
 * Ваши изменения ниже
 */
var requests = ['/countries', '/cities', '/populations'];
var responses = {};

var userAsks = prompt("Enter country or city and I'll tell you it's population", "Africa");

for (i = 0; i < 3; i++) {
    var request = requests[i];
    var callback = (function(request) { return function (error, result) {
        responses[request] = result;
        var l = [];
        for (K in responses)
            l.push(K);

        if (l.length == 3) {
            var countries = [], cities = [], population = 0;
            for (i = 0; i < responses['/countries'].length; i++) {
                if (responses['/countries'][i].continent === userAsks) {
                    countries.push(responses['/countries'][i].name);
                }
                else if ( responses['/countries'][i].name === userAsks)
                    countries.push(responses['/countries'][i].name);
            }

            for (i = 0; i < responses['/cities'].length; i++) {
                if (countries.length > 0)
                    for (j = 0; j < countries.length; j++) {
                            if (responses['/cities'][i].country === countries[j]) {
                            cities.push(responses['/cities'][i].name);
                        }
                    }
                else 
                    if (responses['/cities'][i].name === userAsks) {
                        cities.push(responses['/cities'][i].name);
                    }
            }
            
            for (i = 0; i < responses['/populations'].length; i++) {
                for (j = 0; j < cities.length; j++) {
                    if (responses['/populations'][i].name === cities[j]) {
                        population += responses['/populations'][i].count;
                    }
                }
            }

            alert( population > 0 ? "Population in " + userAsks + ": " +population : "Sorry... Continent/country/city not in database");
        };
    }})(request);
    getData(request, callback);
}

