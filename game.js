// **
//  * @file Physics.py
//  * @brief Javascript file used for the front-end gameplay
//  *
//  **

$(document).ready(function() {
    
    var $table = $('#table');
    var cueBallCenterX, cueBallCenterY;
    var cueStick;
    var isDragging = false;
    var animationInterval;

    function setUp() {
        
        // Create cueball var and find it on the table
        var $cueBall = $table.find('circle[fill="WHITE"]');

        // Store cueball coords
        cueBallCenterX = parseFloat($cueBall.attr('cx'));
        cueBallCenterY = parseFloat($cueBall.attr('cy'));

        $('#stick').remove();

        // Create a cue stick SVG in our HTML
        cueStick = document.createElementNS('http://www.w3.org/2000/svg', 'line');
        cueStick.setAttribute('id', 'stick');

        // set the start of the cue stick to the balls coords
        cueStick.setAttribute('x1', cueBallCenterX);
        cueStick.setAttribute('y1', cueBallCenterY);
        cueStick.setAttribute('x2', cueBallCenterX);
        cueStick.setAttribute('y2', cueBallCenterY);
        cueStick.setAttribute('stroke', 'black');
        cueStick.setAttribute('stroke-width', '20');
        $table.find('svg').append(cueStick);
    }

    // Make sure our cue stick is in visible bounds
    function convertCoord(mouseX, mouseY, cueStick) {
        var point = new DOMPoint(mouseX, mouseY);
        return point.matrixTransform(cueStick.getScreenCTM().inverse());
    }

    // If user clicks and drags, update the position of the end of the cue stick
    function updateCueStick(event) {
     if (isDragging) {
        cueStick.setAttribute('visibility', 'visible');
        var mouseX = event.clientX;
        var mouseY = event.clientY;
        var point = convertCoord(mouseX, mouseY, cueStick)
        cueStick.setAttribute('x2', point.x);
        cueStick.setAttribute('y2', point.y);
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        }
    }

    // When a user has shot, create a next turn
    function createTurn() {
        var $cueBall = $table.find('circle[fill="WHITE"]');
        $cueBall.off('mousedown').on('mousedown', function(event) {
            isDragging = true;
            updateCueStick(event)
            $(document).on('mouseup', function() {
                if (isDragging) {
                    $(document).off('mouseup');
                    isDragging = false;
                    cueStick.setAttribute('visibility', 'hidden');
                    var point = convertCoord(lastMouseX, lastMouseY, cueStick)
                    var velX = (cueBallCenterX - point.x) * 10
                    var velY = (cueBallCenterY - point.y) * 10
                    $.post("/shoot", { velX: velX, velY: velY })
                    .done(function(response) {
                        svgArray = processSVGString(response);
                        animationInterval = setInterval(function () {animate(svgArray)}, 10)
                    })
                    .fail(function(error) {
                        console.error('Error shooting:', error);
                    });
                }
            $(document).off('mouseup');
            });
        });
        $table.off('mousemove').on('mousemove', updateCueStick);

    }

    // Parse our SVG string into an array
    function processSVGString(svgString) {
        var svgArray = svgString.split(',');
        return svgArray
    }

    // Animate our SVG array
    var i = 1;
    function animate(svgArray) {
        if (i < svgArray.length - 1) {
            $('#table').html(svgArray[i]);
            console.log(i)
            i++;
        } else {
            clearInterval(animationInterval);
            i = 1;
            setUp();
            createTurn();
        }
    }

    // main
    function init() {
        setUp();
        createTurn();
    }

    init();


    });
