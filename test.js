;(function(){
    var h = {
        xmlObj:null,
        get:function(arg){
            h.xmlObj = new XMLHttpRequest();
            h.xmlObj.onreadystatechange =function(){
                if (h.xmlObj.readyState === 4 && h.xmlObj.status === 200) {
                    arg.success&&arg.success(h.xmlObj);
                }
            };
            h.xmlObj.open("GET",arg.url,arg.async?arg.async:true);
            h.xmlObj.send();
        }
    };
    window.YCon = {
        openExe:function(name,call){
            h.get({
                url:"http://127.0.0.1:8002/openExe?name="+name,
                success:function(){
                    console.log(arguments);
                }
            })
        }
    };

})();
