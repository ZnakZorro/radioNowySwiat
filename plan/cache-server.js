//https://javascript.plainenglish.io/how-to-make-your-site-25x-faster-with-a-few-lines-of-code-e13056faac2a
const express = require('express')
const app = express();
const redis = require('redis');
const fetch = require('cross-fetch');
const cacheClient = redis.createClient();

let zutPlanURL = "https://wwwapps2.zut.edu.pl/planwi/schedule_student.php?kind=apiwi&department=wi&start=2021-10-11&end=2021-10-12";
let photosUrl  = "https://jsonplaceholder.typicode.com/photos";


app.use(express.static('public'))


app.get('/zutplan', async (req, res, next) => {
await cacheClient.get('zutPlanCache', async (err, data) => {
        if (err) console.error(err);
        if (data) {
            console.log('Data was cached before')
            return res.json({ data: JSON.parse(data) });
        }
        else
          await fetch(zutPlanURL)
                .then(response => response.json())
                .then(json => {
                    console.log('Newly fetched data')
                    cacheClient.setex('zutPlanCache',9000,JSON.stringify(json)); // cache=15minut*10
           return res.json({ data: json });
        })
})
});
app.listen(3000, () => console.log('listening on port 3000'));