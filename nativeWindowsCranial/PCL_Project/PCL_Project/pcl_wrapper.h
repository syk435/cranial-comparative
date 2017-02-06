#ifndef PCL_FEATURES_GLS_CURVATURE_H_
#define PCL_FEATURES_GLS_CURVATURE_H_
#include <pcl/features/feature.h>
#include "Patate/grenaille.h"
#include "Eigen/Eigen"
class GlsPoint
{
public:
    typedef float Scalar;
    typedef Eigen::Matrix<Scalar, 3, 1>   VectorType;
    MULTIARCH inline GlsPoint(Eigen::Map< const VectorType > pos, Eigen::Map< const VectorType > normal)
        : pos_   (pos), 
          normal_(normal)
    {}
    MULTIARCH inline const Eigen::Map< const VectorType >& pos()    const { return pos_; }  
    MULTIARCH inline const Eigen::Map< const VectorType >& normal() const { return normal_; }
private:
    Eigen::Map< const VectorType > pos_, normal_;
};
namespace pcl
{
    template<typename PointInT, typename PointOutT>
    class GlsCurvature : public Feature<PointInT, PointOutT>
    {
        using Feature<PointInT, PointOutT>::feature_name_;
        using Feature<PointInT, PointOutT>::getClassName;
        using Feature<PointInT, PointOutT>::indices_;
        using Feature<PointInT, PointOutT>::input_;
        using Feature<PointInT, PointOutT>::surface_;
        using Feature<PointInT, PointOutT>::k_;
        using Feature<PointInT, PointOutT>::search_radius_;
        using Feature<PointInT, PointOutT>::search_parameter_;
        typedef typename Feature<PointInT, PointOutT>::PointCloudOut PointCloudOut;
        typedef typename Feature<PointInT, PointOutT>::PointCloudConstPtr PointCloudConstPtr;
    public:
        GlsCurvature()
        {
            feature_name_ = "GlsCurvature";
        }
        void computeCurvature(const pcl::PointCloud<PointInT> &cloud, int p_idx, const std::vector<int> &indices, float &curvature);
    protected:
        void
        computeFeature (PointCloudOut &output);
    private:
        void
        computeFeatureEigen (pcl::PointCloud<Eigen::MatrixXf> &) {}
    };
}
#endif // PCL_FEATURES_GLS_CURVATURE_H_