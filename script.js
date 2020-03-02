Chart.plugins.unregister(ChartDataLabels);

function myFunction(){
var data;

var cells = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16];
var voltages = [];
var colours =[];
function getColour(voltage){
    if(voltage<=3){return "#DB0000"}
    else{
      return "#00BC1D"
    };
}

var ctx = document.getElementById("voltageChart");
var voltageChart = new Chart(ctx, {
          plugins:[ChartDataLabels],
          type: "bar",
          responsive: true,
          data: {
            labels: cells,
            datasets: [
              {
                data: voltages,
                backgroundColor: colours,
              }
            ]
          },
          options: {
            responsive: true,
            maintainAspectRatio: false,
            title: {
              display: true,
              text: 'Cell Voltages',
              padding: 10,
              fontSize: 40,
              fontColor: 'black',
              fontFamily: 'Helvetica Neue, Arial, sans-serif',
            },
            plugins: {
					         datalabels: {

                      anchor: 'end',
                      align: 'end',
                      /* Adjust data label font size according to chart size */
                      font: function(context) {
                              var width = context.chart.width;
                              var size = Math.round(width / 40);

                              return {
                                weight: 'normal',
                                size: size
                              };
                            }
					          }
				    },
            legend: {
              display: false
            },
            tooltips: {
              enabled: false
            },
            scales: {
              xAxes: [{
                gridLines: {
                  display:false
                },
              }],
              yAxes: [{
                gridLines: {
                display:false
                },
                ticks: {
                  min: 2.5,
                  max: 3.6,
                  display: false,
                },
                // ticks: {
                //   fontSize: 20,
                //   suggestedMin: 2.5,
                //   suggestedMax: 3.5,
                //   stepSize: 0.5,
                //   callback: function(value, index, values) {
                //         return value + ' V';
                //     },
                //   display: true,
                // },
              }]
            },
          },
        });

    function updateData(voltagesNew, coloursNew) {
    voltageChart.data.datasets[0].data = voltagesNew;
    voltageChart.data.datasets[0].backgroundColor = coloursNew;
    voltageChart.update();
    }


    async function refreshData() {
      response = await fetch('http://esp8266.local/values.json')
      if (response.ok) { // if HTTP-status is 200-299
        console.log(response);
        data = await response.json();
        console.log(data);
      } else {
        alert("HTTP-Error: " + response.status);
      }

      document.getElementById("bemuid").innerHTML = data.BEMUID;
      document.getElementById("voltageID").innerHTML = data.voltage + ' V';
      document.getElementById("currentID").innerHTML = data.current + ' A';

      var chg = document.getElementById("myprogressBar");
      var pctCharge = data.charge;
      chg.style.width = pctCharge + '%';
      chg.innerHTML =  pctCharge*1 + '%';

      var voltages = data["cell voltages"];
      for(i = 0; i < voltages.length; i++){
        colours[i] = getColour(voltages[i]);
      }
      updateData(voltages, colours);
      }
 setInterval(refreshData, 1500);
}
