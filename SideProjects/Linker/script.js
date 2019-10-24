var opennew = false;

function jump(){
    let ID = document.getElementById("id-input").value;
    let PF = document.getElementById("pf-input").value;
    let href = 'https://';

    switch(PF){
        case '':
            return;
        case 'NH': 
            href += 'nhentai.net/g/';
            href += ID += '/1/';
            break;
        case 'HDM':
            href += 'ahri.date/readOnline2.php?ID=';
            href += ID += '&host_id=0';
            break;
        case '177':
            href += 'www.177pic.info/html/';
            href += ID += '.html';
            break;
    }

    window.open(href, '_blank');
}