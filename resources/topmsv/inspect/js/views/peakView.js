"use strict";
/**
 * Set Peak List on to html
 */
function setDataToPeakAndIntensity(peakAndIntensityList) {
    jqueryElements.peakData.val(peakAndIntensityList);
}
/**
 * Function to get data of peaks and intensity from UI
 */
function getPeakListFromUI() {
    let spectrumDataList = [];
    // Read data line by line from peak and intensity box
    let peakData = jqueryElements.peakData.val();
    if (typeof (peakData) == 'string') {
        var lines = peakData.split('\n');
        for (var i = 0; i < lines.length; i++) {
            let peakAndInte = lines[i].trim();
            if (peakAndInte.length !== 0) {
                let peakInte = peakAndInte.split(/[\s]+/);
                if (peakInte[0] != undefined && peakInte[1] != undefined
                    && !isNaN(parseFloat(peakInte[0])) && !isNaN(parseFloat(peakInte[1]))) {
                    let peak = new Peak(i.toString(), parseFloat(peakInte[0]), parseFloat(peakInte[0]), parseFloat(peakInte[1]));
                    if (peak.getPos() !== undefined && peak.getIntensity() !== undefined) {
                        spectrumDataList.push(peak);
                    }
                }
            }
        }
    }
    // completeCalData.peakdatalist = spectrumDataList;
    return spectrumDataList;
}
