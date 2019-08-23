//
//  graphCell.swift
//  tableView
//
//  Created by Thomas Sloan on 2019-07-06.
//  Copyright © 2019 Thomas Sloan. All rights reserved.
//

import UIKit
import Charts

class graphCell: UITableViewCell {

    @IBOutlet var titleView: UIView!
    @IBOutlet var view: UIView!
    @IBOutlet var lineChartView: LineChartView!
    @IBOutlet var titleLabel: UILabel!
    override func awakeFromNib() {
        
        super.awakeFromNib()
        
        // Initialization code
        setChartValues()
        
        titleView.backgroundColor = .clear
        titleLabel.textColor = .white
        view.addGestureRecognizer(UITapGestureRecognizer(target: self, action: #selector(handleTap)))
    }
    
    @objc private func handleTap(){
        print("TAPPED")
    }
    override func setSelected(_ selected: Bool, animated: Bool) {
        super.setSelected(selected, animated: animated)

        // Configure the view for the selected state
    }
    func setChartValues(_ count : Int = 20) {
        let values = (0..<count).map { (i) -> ChartDataEntry in
            let val = Double(arc4random_uniform(UInt32(count)) + 3)
            return ChartDataEntry(x: Double(i), y: val)
        }
        
        let set1 = LineChartDataSet(entries: values, label: "DataSet 1")
        let data = LineChartData(dataSet: set1)
        lineChartView.legend.textColor = UIColor.white
        lineChartView.gridBackgroundColor = UIColor.white
        self.lineChartView.data = data
    
        self.view.layer.cornerRadius = 20.0
        self.view.layer.shadowColor = UIColor.gray.cgColor
        self.view.layer.shadowOffset = CGSize(width: 0.0, height: 0.0)
        self.view.layer.shadowRadius = 5.0
        self.view.layer.shadowOpacity = 0.7
        
    }
    
}



