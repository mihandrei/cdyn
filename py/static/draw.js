(function(){

var style = {
    str:colorbrewer.Blues[3][1],
    alt:colorbrewer.Blues[3][0],
    ax:colorbrewer.Blues[3][2]
};

function Drawe(){
    var body = document.getElementsByTagName('body')[0];
    this.W = Math.min(body.offsetWidth, body.offsetHeight);
    this.c = document.getElementById('ploty');
    this.c.width = this.W-4;
    this.c.height = this.W-4;
}

function grid(ctx, d, w, h){
    d = d || 40;
    w = w || 100;
    h = h || 100;
    for (var i = 0; i < d; i++) {
      var x = 0.5 + (i - d/2)*(2*w/d);
      var y = 0.5 + (i - d/2)*(2*h/d);
      ctx.beginPath();
      ctx.moveTo(x, -w);
      ctx.lineTo(x, w);
      ctx.moveTo(-h, y);
      ctx.lineTo(h, y);
      if (i % 5 == 0){
         ctx.strokeStyle = style.str;
      }else{
         ctx.strokeStyle = style.alt;
      }
      ctx.stroke();
    }

    ctx.beginPath();
    ctx.moveTo(0.5, -w+1);
    ctx.lineTo(0.5, w-1);
    ctx.moveTo(-h+1, 0.5);
    ctx.lineTo(h-1, 0.5);
    ctx.strokeStyle = style.ax;
    ctx.stroke();
};

Drawe.prototype.draw = function(t){
    var ctx = this.c.getContext("2d");
    ctx.clearRect(0,0, 600, 600);
    ctx.save();
    ctx.translate(300, 300);
    //grid(ctx, 100, 300, 300);
    ctx.fillStyle = 'rgba(200, 10, 50, 0.6)';

    var frame;
    var n, v, a;
    frame = data.lin[time];
    for(n=0; n < frame.length; n++){
        a = frame[n][0];
        ctx.fillRect(-300 + 20*n , -270+ 20*a, 10, 10);
    }

    ctx.fillStyle = 'rgba(0, 20, 60, 0.6)';
    frame = data.grid[time];
    for(n=0; n < 5; n++){//! hardcoded grid shape
        for(v=0; v < 5; v++) {
            a = frame[n * 5 + v][0];
            var d = 18*(1 + a);
            ctx.fillRect(-300 + 20 * v - d/2, -240 + 20*n - d/2, d, d);
        }
    }
    ctx.restore();
    //requestAnimationFrame(function(t){self.draw(t);});
};

window.Drawe = Drawe;

})();

var data = [], time = 0;

$(document).ready(function(){
    var d = new Drawe();
    $.get('/step', function(rdata){
        data = JSON.parse(rdata);
        var timer = setInterval(function(){
            d.draw();
            time +=1;
            if (time >= data.lin.length||time>=data.grid.length){
                clearInterval(timer);
            }
        }, 50);
    });
});
