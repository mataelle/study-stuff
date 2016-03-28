(function(){
	
    function map(x, in_min, in_max, out_min, out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }

	function makeDraggable(slider, $handle) {
        $handle.draggable({
            containment: slider.$base,
            scroll: false,
            axis: "x"
        });
        $handle.on('drag', function() {
            slider.setValueFromPosition();
            if (slider.onDragCallback) {
                slider.onDragCallback();
            }
        });
    }
	
	function RangeSlider(selector, params) {
		this.$element = $(selector);
		this.$base = $('<div class="rangeslider-base">');
        this.$leftHandle = $('<div class="rangeslider-handle-left">');
        this.$rightHandle = $('<div class="rangeslider-handle-right">');
        this.$leftCaption = $('<span class="rangeslider-caption">').text(params.start[0]);
        this.$rightCaption = $('<span class="rangeslider-caption">').text(params.start[1]);
		
		this.$base.append(this.$leftHandle.append(this.$leftCaption))
                  .append(this.$rightHandle.append(this.$rightCaption));
        this.$element.append(this.$base);

        makeDraggable(this, this.$leftHandle);
        makeDraggable(this, this.$rightHandle);

        this.rangeWidth = this.$base.width() - this.$leftHandle.width();
        this.range = params.range;
        this.value(params.start);
        this.onDragCallback = params.onDrag;
	}
     RangeSlider.prototype.value = function(value) {
        if (value) {
            this.$leftHandle.css('left',
                map(value[0], this.range[0], this.range[1], 0, this.rangeWidth));
            this.$rightHandle.css('right',
                map(value[1], this.range[0], this.range[1], this.rangeWidth, 0));
            this.setValueFromPosition();
            return this;
        } else {
            return this.numberValue;
        }
    };

    RangeSlider.prototype.setValueFromPosition = function() {
        var self = this;

        function to_range(x) {
            return Math.round(map(x, 0, self.rangeWidth, self.range[0], self.range[1]));
        }

        var leftPos = to_range(self.$leftHandle.position().left);
        var rightPos = to_range(self.$rightHandle.position().left);
        this.$leftCaption.text(leftPos);
        this.$rightCaption.text(rightPos);
        this.numberValue = [leftPos, rightPos];    
    };
	window.RangeSlider = RangeSlider;
})();